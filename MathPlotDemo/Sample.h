/*
 * Sample.cpp
 *
 *  Created on: 4 avr. 2023
 *      Author: Lionel
 *
 *  This is the sample of the official wxMathplot widget
 */

#include "mathplot.h"

/**
 * Classic sinus curve
 */
class MySIN: public mpFX
{
  public:
    MySIN(double freq, double amp) :
        mpFX(wxT("f(x) = SIN(x)"), mpALIGN_LEFT)
    {
      m_freq = freq;
      m_amp = amp;
      wxPen FXpen(*wxGREEN, 1, wxPENSTYLE_SOLID);
      SetDrawOutsideMargins(true);
      SetContinuity(false);
      SetPen(FXpen);
      SetSymbol(mpsCircle);
      SetStep(8);
    }
    virtual double GetY(double x)
    {
      return m_amp * sin(x / 6.283185 / m_freq);
    }
    virtual double GetMinY()
    {
      return -m_amp;
    }
    virtual double GetMaxY()
    {
      return m_amp;
    }
  protected:
    double m_freq, m_amp;
};

class MyFunction: public mpFXGeneric
{
  public:
    MyFunction() :
      mpFXGeneric(wxT("f(x) = x/sqrt(sin(x)+2)"), mpALIGN_LEFT)
    {
      wxPen FXpen(*wxBLUE, 1, wxPENSTYLE_SOLID);
      SetDrawOutsideMargins(true);
      SetContinuity(false);
      SetPen(FXpen);
      SetSymbol(mpsSquare);
    }

  protected:
    virtual double ComputeY(double x)
    {
      return x / (sqrt(sin(x) + 2));
    }
};

class MyPower: public mpFXGeneric
{
  public:
    MyPower() :
      mpFXGeneric(wxT("f(x) = 10^x"), mpALIGN_LEFT)
    {
      SetContinuity(false);
      SetSymbol(mpsSquare);
    }

  protected:
    virtual double ComputeY(double x)
    {
      return pow(10, x);
    }
};

// MyCOSinverse
class MyCOSinverse: public mpFY
{
  public:
    MyCOSinverse(double freq, double amp) :
        mpFY(wxT("g(y) = COS(y)"), mpALIGN_BOTTOM)
    {
      m_freq = freq;
      m_amp = amp;
      wxPen FYpen(*wxCYAN, 2, wxPENSTYLE_SOLID);
      SetDrawOutsideMargins(false);
      SetContinuity(true);
      SetPen(FYpen);
      SetSymbol(mpsSquare);
      SetSymbolSize(8);
      SetStep(8);
    }
    virtual double GetX(double y)
    {
      return m_amp * cos(y / 6.283185 / m_freq);
    }
    virtual double GetMinX()
    {
      return -m_amp;
    }
    virtual double GetMaxX()
    {
      return m_amp;
    }
  protected:
    double m_freq, m_amp;
};

// MyLissajoux
class MyLissajoux: public mpFXY
{
    double m_rad;
    int m_idx;
  public:
    MyLissajoux(double rad) :
        mpFXY(wxT("Lissajoux"))
    {
      m_rad = rad;
      m_idx = 0;
      wxPen FXYpen(*wxRED, 1, wxPENSTYLE_SOLID);
      SetDrawOutsideMargins(false);
      SetContinuity(false);
      SetPen(FXYpen);
      SetSymbol(mpsCross);
    }
    virtual bool GetNextXY(double* x, double* y)
    {
      if (m_idx < 360)
      {
        *x = m_rad * cos(m_idx / 6.283185 * 360);
        *y = m_rad * sin(m_idx / 6.283185 * 360 * 3);
        m_idx++;
        return true;
      }
      else
      {
        return false;
      }
    }
    virtual void Rewind()
    {
      m_idx = 0;
    }
    virtual double GetMinX()
    {
      return -m_rad;
    }
    virtual double GetMaxX()
    {
      return m_rad;
    }
    virtual double GetMinY()
    {
      return -m_rad;
    }
    virtual double GetMaxY()
    {
      return m_rad;
    }
};

class FixedBitwidth: public mpFX
{
    int m_bitwidth;
  public:
    FixedBitwidth(int bitwidth) :
        mpFX(wxT("Feste Bitbreite"))
    {
      m_bitwidth = bitwidth;
    }

    virtual double GetY(double x)
    {
      if (x >= 0)
        return m_bitwidth;
      else
        return 0;
    }
};

double ld(const double x)
{
  return log(x) / log(2.0f);
}

class Optimum: public mpFX
{
  public:
    Optimum() :
        mpFX(wxT("Optimum ld(N)"))
    {
    }

    virtual double GetY(double x)
    {
      if (x >= 1)
        return ld(x);
      else
        return 0;
    }
};

class Elias: public mpFX
{
  public:
    Elias() :
        mpFX(wxT("Elias-Kodes"))
    {
    }

    virtual double GetY(double x)
    {
      if (x >= 1)
        return floor(ld(x)) + 2.0 * floor(ld(1.0 + floor(ld(x))));
      else
        return 0;
    }
};

class Fibonacci: public mpFX
{
  public:
    Fibonacci() :
        mpFX(wxT("Fibonacci-Kodes"))
    {
    }

    virtual double GetY(double N)
    {
      if (N >= 1)
      {
        /*
         for (DWORD mask = 0; mask<N; mask++)
         {
         for (int i=0; i<N; i++)
         {
         if (mask

         }
         }
         */

        return N;
      }
      else
        return 0;
    }
};
