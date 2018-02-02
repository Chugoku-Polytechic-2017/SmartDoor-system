#include "capture.h"

#define Y1 YUYV[0]
#define U  YUYV[1]
#define Y2 YUYV[2]
#define V  YUYV[3]

#define CLEAR(x) memset(&(x), 0, sizeof (x))

#define IMG_WIDTH 	320
#define IMG_HEIGHT 	240

static void errno_exit(const char *s);
static int xioctl(int fd, int request, void * arg);
static void process_image(const void *p);
static int read_frame(void);
static void mainloop(void);
static void stop_capturing(void);
static void start_capturing(void);
static void init_mmap(void);
static void uninit_device(void);
static void init_device(void);
static void close_device(void);
static void open_device(void);
void convert(char out[128]);

struct buffer {
	void 	*start;
	size_t   	length;
};

static char *dev_name	= "/dev/video0";	// ビデオデバイス名
static int  fd	 	= -1;			// ビデオデバイスのハンドラ
struct buffer *buffers	= NULL;			// バッファへの先頭アドレス
static unsigned int n_buffers = 0;		// バッファ数
static char output_file[128] = "sample.yuv";			// 出力ファイル
static char filename[128] = "face.jpg";


static void errno_exit(const char *s)
{
	fprintf(stderr,"%s error %d, %s\n", s, errno, strerror (errno));
	exit (EXIT_FAILURE);
}

static int xioctl(int fd, int request, void * arg)
{
	int r;
	
	do {
		r = ioctl(fd, request, arg);
	} while (-1 == r && EINTR == errno);
	return r;
}

static void process_image(const void *p)	
{
	
	//アドレスpにYUVのイメージ画像が入っているので
	//ファイルに書き込めばよい
	//ファイルオープン、書き込み、ファイルクローズ
	FILE *fp;
	if((fp = fopen( output_file, "wb" )) == NULL)
	{
		perror("output_open");
	}
	fwrite(p, 153600, n_buffers, fp);
	convert(output_file);
	fclose(fp);
}


static int read_frame(void)
{
	struct v4l2_buffer buf;

	CLEAR (buf);
	buf.type 	 = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory    = V4L2_MEMORY_MMAP;
	// outgoing queue から1フレーム分のバッファを取り出す
	if (xioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
		switch (errno) {	
			case EAGAIN:		// outgoing queueが空のとき
				return 0;
			default:
				errno_exit("VIDIOC_DQBUF");
		}
	}
	assert(buf.index < n_buffers);
	process_image(buffers[buf.index].start); // フレームを処理
	xioctl(fd, VIDIOC_QBUF, &buf);	   // 使用したバッファをincoming queueに入れる

	return 1;
}

static void mainloop(void)
{
	unsigned int count;
	count = 1;				// captureするフレーム数を指定

	while (count-- > 0) {
		for (;;) {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);
			tv.tv_sec = 5;		// タイムアウト時間を設定
			tv.tv_usec = 0;
			r = select(fd+1, &fds, NULL, NULL, &tv);  // USBカメラからのイベント待ち

			if (r == -1) {		// エラー処理
				if (EINTR == errno) {
					continue;
				}
				errno_exit("select");
			}
			if (r == 0) {			// タイムアウト処理
				fprintf (stderr, "select timeout\n");
				exit (EXIT_FAILURE);
			}
			if (read_frame()){	// 読み込みOKになったらフレームを1枚読み込む
				printf(".");
				break;	//ループ脱出
			} else {
				printf("EAGAIN\n");
			}
			// EAGAIN : outgoing queueが空のとき繰り返す
		}
	}
}

static void stop_capturing(void)
{
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	xioctl(fd, VIDIOC_STREAMOFF, &type);	// ストリーミングを停止
}

static void start_capturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;		// バッファの識別子
		xioctl(fd, VIDIOC_QBUF, &buf);	// incoming queueにバッファをエンキュー
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	xioctl(fd, VIDIOC_STREAMON, &type);	//　ストリーミングを開始
}

static void uninit_device(void)
{
	unsigned int i;

	for (i = 0; i < n_buffers; ++i) {
		munmap(buffers[i].start, buffers[i].length);	// アンマップ
	}
	free(buffers);					// メモリを開放
}

static void init_mmap(void)
{
	struct v4l2_requestbuffers req;

	CLEAR (req);
	req.count	= 4;							// バッファ数
	req.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;	// バッファタイプ
	req.memory	= V4L2_MEMORY_MMAP;				// MMAPを使用
	xioctl(fd, VIDIOC_REQBUFS, &req);			// メモリマッピングの設定

	buffers = calloc(req.count, sizeof(*buffers));	//　動的にメモリを確保

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory  = V4L2_MEMORY_MMAP;
		buf.index   = n_buffers;
		xioctl(fd, VIDIOC_QUERYBUF, &buf);

		buffers[n_buffers].length = buf.length;
		//　デバイスメモリとアプリケーションのメモリをマッピング（共有）			
		buffers[n_buffers].start = mmap(NULL,	// start anywhere
			buf.length,							// バッファの長さ
			PROT_READ | PROT_WRITE,				// メモリ保護を指定
			MAP_SHARED,							// 共有を指定
			fd, buf.m.offset);					// ファイルハンドラ、オフセット
	}
}
	
static void init_device(void)
{
	struct v4l2_format fmt;

	CLEAR(fmt);
	fmt.type 		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width	= IMG_WIDTH; 	     // キャプチャ画像の解像度：幅
	fmt.fmt.pix.height	= IMG_HEIGHT;	     // キャプチャ画像の解像度：高さ
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;// ピクセルフォーマットの設定：YUYV
	fmt.fmt.pix.field  = V4L2_FIELD_INTERLACED; // 受信するビデオフォーマットの設定
	xioctl(fd, VIDIOC_S_FMT, &fmt);	     // 映像フォーマットの設定

	init_mmap();	// メモリマッピングの初期化
}
	
static void close_device(void)
{
	close(fd);
}
	
static void open_device(void)
{
	fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
}
	
void convert(char out[128])
{

	// JPEGオブジェクト, エラーハンドラを確保
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int i, j;
	unsigned char YUYV[4];

	int Ri;
	int Gi;
	int Bi;

	unsigned char Rc;
	unsigned char Gc;
	unsigned char Bc;

	// 出力ファイルのハンドラを確保、出力ファイル名を指定
	FILE *fp;
	FILE *fp_yuv;

	// 画像のサイズ
	int width = IMG_WIDTH;
	int height = IMG_HEIGHT;

	//エラーハンドラにデフォルト値を設定
	cinfo.err = jpeg_std_error(&jerr);

	// JPEGオブジェクトを初期化
	jpeg_create_compress(&cinfo);

	// 出力ファイルをオープン
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "cannot open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	// 出力ファイルを指定
	jpeg_stdio_dest(&cinfo, fp);

	// 画像のパラメータを設定
	cinfo.image_width = width;

	// 画像の幅
	cinfo.image_height = height;

	// 画像の高さ
	cinfo.input_components = 3;

	// 1pixelあたりのカラー要素数
	cinfo.in_color_space = JCS_RGB;

	// 入力画像の形式
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 75, TRUE);

	// 圧縮効率（0～100）
	// 圧縮を開始
	jpeg_start_compress(&cinfo, TRUE);

	if ((fp_yuv = fopen(out, "rb")) == NULL)
	{
		fprintf(stderr, "cannot open %s\n", out);
		exit(EXIT_FAILURE);
	}

	// データ（RGB値）を生成
	JSAMPARRAY img = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);
	for (i = 0; i < height; i++)
	{
		img[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * 3 * width);
		for (j = 0; j < width; j++)
		{
			if((j % 2) == 0){
				fread(YUYV, 1, 4, fp_yuv);
				Ri = 1.164 * (Y1 - 16) + 1.569 * (V - 128);
				Gi = 1.164 * (Y1 - 16) - 0.391 * (U - 128) - 0.813 * (V - 128);
				Bi = 1.164 * (Y1 - 16) + 2.018 * (U - 128);
			}else{
				Ri = 1.164 * (Y2 - 16) + 1.569 * (V - 128);
				Gi = 1.164 * (Y2 - 16) - 0.391 * (U - 128) - 0.813 * (V - 128);
				Bi = 1.164 * (Y2 - 16) + 2.018 * (U - 128);
			}

			if( Ri < 0 ){
			Rc = 0;
			}else if( Ri > 255 ){
			Rc = 255;
			}else{
			Rc = Ri;
			}

			if( Gi < 0 ){
			Gc = 0;
			}else if( Gi > 255 ){
			Gc = 255;
			}else{
			Gc = Gi;
			}

			if( Bi < 0 ){
			Bc = 0;
			}else if( Bi > 255 ){
			Bc = 255;
			}else{
			Bc = Bi;
			}

			img[i][j * 3 + 0] = Rc;
			img[i][j * 3 + 1] = Gc;
			img[i][j * 3 + 2] = Bc;
		}
	}

	// 1行でなく画像全体を出力
	jpeg_write_scanlines(&cinfo, img, height);

	// 圧縮を終了
	jpeg_finish_compress(&cinfo);

	// JPEGオブジェクトを破棄
	jpeg_destroy_compress(&cinfo);

	for (i = 0; i < height; i++)
	{
		free(img[i]);
	}

	free(img);
	fclose(fp);
}


int caputure()
{
	strcpy(filename, argv[1]);
	open_device();		//　ビデオバイスをオープン
	init_device();		//　ビデオデバイスを初期化
	start_capturing();	//　画像キャプチャ開始
	mainloop();			//　メインループ処理
	stop_capturing();	//　画像キャプチャ停止
	uninit_device();	//　初期化前の状態に戻す
	close_device();		//　ビデオデバイスをクローズ
	system("rm -rf sample.yuv");
	return 0;
}
