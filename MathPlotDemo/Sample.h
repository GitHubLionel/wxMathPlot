/*
 * Sample.cpp
 *
 *  Created on: 4 avr. 2023
 *      Author: Lionel
 *
 *  This is the sample of the official wxMathplot widget
 */

#include "mathplot.h"

class MySIN: public mpFX
{
		double m_freq, m_amp;
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
			SetStep(4);
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
};

class MyFunction: public mpFX
{
		double minY, maxY;

	public:
		MyFunction() :
				mpFX(wxT("f(x) = x/sqrt(sin(x)+2)"), mpALIGN_LEFT)
		{
			minY = 0;
			maxY = 0;

			wxPen FXpen(*wxBLUE, 1, wxPENSTYLE_SOLID);
			SetDrawOutsideMargins(true);
			SetContinuity(false);
			SetPen(FXpen);
			SetSymbol(mpsSquare);
			SetStep(4);
		}
		virtual double GetY(double x)
		{
			double y;
			try
			{
				y = x / (sqrt(sin(x) + 2));
			} catch (...)
			{
				y = 0;
			}
			if (y < minY)
				minY = y;
			else
				if (y > maxY)
					maxY = y;
			return y;
		}
		virtual double GetMinY()
		{
			return minY;
		}
		virtual double GetMaxY()
		{
			return maxY;
		}
		virtual void DoBeforePlot()
		{
			minY = 0;
			maxY = 0;
		}
};

// MyCOSinverse

class MyCOSinverse: public mpFY
{
		double m_freq, m_amp;
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
		virtual bool GetNextXY(double &x, double &y)
		{
			if (m_idx < 360)
			{
				x = m_rad * cos(m_idx / 6.283185 * 360);
				y = m_rad * sin(m_idx / 6.283185 * 360 * 3);
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

