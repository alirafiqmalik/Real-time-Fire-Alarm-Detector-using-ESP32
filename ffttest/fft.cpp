#include <math.h>
#include <algorithm>

#define twoPi 6.28318531
#define fourPi 12.56637061
#define sixPi 18.84955593

static const int FFT_FORWARD = 0x01;
static const int FFT_REVERSE = 0x00;

static const int FFT_WIN_TYP_RECTANGLE = 0x00;
static const int FFT_WIN_TYP_HAMMING = 0x01;
static const int FFT_WIN_TYP_HANN = 0x02;
static const int FFT_WIN_TYP_TRIANGLE = 0x03;
static const int FFT_WIN_TYP_NUTTALL = 0x04;
static const int FFT_WIN_TYP_BLACKMAN = 0x05;
static const int FFT_WIN_TYP_BLACKMAN_NUTTALL = 0x06;
static const int FFT_WIN_TYP_BLACKMAN_HARRIS = 0x07;
static const int FFT_WIN_TYP_FLT_TOP = 0x08;
static const int FFT_WIN_TYP_WELCH = 0x09;

class arduinoFFT
{
public:
  /* Constructor */
  //	arduinoFFT(void);
  arduinoFFT(float *vReal, float *vImag, int samples, float samplingFrequency);
  /* Destructor */
  ~arduinoFFT(void);
  /* Functions */
  int Revision(void);
  int Exponent(int value);
  //	void ComplexToMagnitude(float *vReal, float *vImag, int samples);
  //	void Compute(float *vReal, float *vImag, int samples, int dir);
  //	void Compute(float *vReal, float *vImag, int samples, int power, int dir);
  //	void DCRemoval(float *vData, int samples);
  //	float MajorPeak(float *vD, int samples, float samplingFrequency);
  //	void Windowing(float *vData, int samples, int windowType, int dir);
  void ComplexToMagnitude();
  void Compute(int dir);
  void DCRemoval();
  float MajorPeak();
  void Windowing(int windowType, int dir);


void _bit_reverse();
int _bit_reverse_index(int i);
void _fft(int dir);


  void MajorPeak(float *f, float *v);
  //	void MajorPeak(float *vD, int samples, float samplingFrequency, float *f, float *v);

private:
  /* Variables */
  int samples;
  float samplingFrequency;
  float *vReal;
  float *vImag;
  int power;
  /* Functions */
  // void Swap(float *x, float *y);
};



int arduinoFFT::Exponent(int value)
{
    int result = 0;
    while (((value >> result) & 1) != 1) result++;
    return result;
}


arduinoFFT::arduinoFFT(float *vReal, float *vImag, int samples, float samplingFrequency)
{ // Constructor
  this->vReal = vReal;
  this->vImag = vImag;
  this->samples = samples;
  this->samplingFrequency = samplingFrequency;
  this->power = Exponent(samples);
}

arduinoFFT::~arduinoFFT(void)
{
  // Destructor
}

void arduinoFFT::Windowing(int windowType, int dir)
{
  float samplesMinusOne = (float(this->samples) - 1.0);
  for (int i = 0; i < (this->samples >> 1); i++)
  {
    float indexMinusOne = float(i);
    float ratio = (indexMinusOne / samplesMinusOne);
    float weighingFactor = 1.0;
    switch (windowType)
    {
    case FFT_WIN_TYP_FLT_TOP:
      weighingFactor = 0.2810639 - (0.5208972 * cos(twoPi * ratio)) + (0.1980399 * cos(fourPi * ratio));
      break;
    }
    if (dir == FFT_FORWARD)
    {
      this->vReal[i] *= weighingFactor;
      this->vReal[this->samples - (i + 1)] *= weighingFactor;
    }
    else
    {
      this->vReal[i] /= weighingFactor;
      this->vReal[this->samples - (i + 1)] /= weighingFactor;
    }
  }
}

void arduinoFFT::_bit_reverse()
{
  for (int i = 0; i < this->samples; i++)
  {
    int j = this->_bit_reverse_index(i);
    if (i < j)
    {
      std::swap(this->vReal[i], this->vReal[j]);
      std::swap(this->vImag[i], this->vImag[j]);
    }
  }
}

int arduinoFFT::_bit_reverse_index(int i)
{
  int n = this->samples;
  int j = 0;
  for (int k = 0; k < log2(n); k++)
  {
    j = (j << 1) | (i & 1);
    i >>= 1;
  }
  return j;
}

void arduinoFFT::_fft(int dir)
{
  int n = this->samples;
  int l = log2(n);
  for (int i = 0; i < n; i++)
  {
    int j = this->_bit_reverse_index(i);
    if (i < j)
    {
      std::swap(this->vReal[i], this->vReal[j]);
      std::swap(this->vImag[i], this->vImag[j]);
    }
  }
  for (int s = 1; s <= l; s++)
  {
    int m = 1 << s;
    float wm_real = cos(twoPi / m);
    float wm_imag = dir * sin(twoPi / m);
    for (int k = 0; k < n; k += m)
    {
      float w_real = 1;
      float w_imag = 0;
      for (int j = k; j < k + m / 2; j++)
      {
        float t_real = w_real * this->vReal[j + m / 2] - w_imag * this->vImag[j + m / 2];
        float t_imag = w_real * this->vImag[j + m / 2] + w_imag * this->vReal[j + m / 2];
        this->vReal[j + m / 2] = this->vReal[j] - t_real;
        this->vImag[j + m / 2] = this->vImag[j] - t_imag;
        this->vReal[j] += t_real;
        this->vImag[j] += t_imag;
        t_real = w_real * wm_real - w_imag * wm_imag;
        t_imag = w_real * wm_imag + w_imag * wm_real;
        w_real = t_real;
        w_imag = t_imag;
      }
    }
  }
}

void arduinoFFT::Compute(int dir)
{
  this->_bit_reverse();
  this->_fft(dir);
}
