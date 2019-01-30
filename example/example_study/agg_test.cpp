#include <windows.h>
 
#include "agg_platform_support.h"
#include "agg_win32_bmp.h" 

#include "agg_rendering_buffer.h"

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_scanline_u.h"
#include "agg_ellipse.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_rasterizer_scanline_aa.h"

#include <vector>
//------------------------------

struct 	Point2D
{
	float x;
	float y;

	Point2D()
		:x(0.0f),y(0.0f)
	{}

	Point2D(float xx, float yy)
		:x(xx), y(yy)
	{}
};


class the_application : public  platform_support
{


public:
	the_application() 
	{

	}

	void on_init()
	{
		m_ctrlIndex =-1;
		
		m_ctrlPoints.push_back(Point2D(157.0, 298.0));
		m_ctrlPoints.push_back(Point2D(305.0, 150.0));
		m_ctrlPoints.push_back(Point2D(506.0, 151.0));
		m_ctrlPoints.push_back(Point2D(588.0, 293.0)); 

		BezierCurve(m_samplePoints, m_ctrlPoints);
	}

	void on_mouse_move(int x, int y, int flags)
	{
		int nCtrCnt =m_ctrlPoints.size();
		if(m_ctrlIndex >= 0 && m_ctrlIndex < nCtrCnt)
		{
			m_ctrlPoints[m_ctrlIndex].x = x;
			m_ctrlPoints[m_ctrlIndex].y = y;

			BezierCurve(m_samplePoints, m_ctrlPoints);
			
			invaldate(true);
		}
	}

	void on_left_button_down(int x, int y, int flags)
	{	
		int nCtrCnt =m_ctrlPoints.size();
		for (int i = 0; i < nCtrCnt; i++)			
		{
			if(m_ctrlPoints[i].x-5 < x && m_ctrlPoints[i].x+5 > x && 
			   m_ctrlPoints[i].y-5 < y && m_ctrlPoints[i].y+5 > y )
			{
				m_ctrlIndex = i;
				break;
			}
		}
	}

	void on_left_button_up(int x, int y, int  flags)
	{
		m_ctrlIndex = -1;
	}

	void on_resize(int sx, int sy)
	{
		m_pixel_map.create(sx, sy, pixel_map::org_color24);

		m_renbuf.attach(m_pixel_map.buf(), 
			m_pixel_map.width(),
			m_pixel_map.height(),
			-m_pixel_map.stride());


	}
	
	int Combinatory(int n, int k)
	{
		int fracK = 1;

		for (int i = 1; i <= k; i++)
		{
			fracK *= i;
		}

		int nPermutation = 1;
		for (int i = n - k + 1; i <= n; i++)
		{
			nPermutation *= i;
		}

		return nPermutation / fracK;
	}

	int* CalcCoefficientMatrix(int degree)
	{
		int n = degree - 1;

		int* Coefficient = new int[degree * degree];
		memset(Coefficient, 0, degree*degree*sizeof(int));


		//¼ÆËã¾ØÕó
		for (int i = 0; i < degree; i++)
		{
			for (int k = i; k < degree; k++)
			{
				int iCNK = Combinatory(n, k);
				int iCKI = Combinatory(k, i);

				int nValue = iCNK*iCKI;
				if ((i + k) % 2)
				{
					nValue = -nValue;
				}

				Coefficient[k * degree + i] = nValue;
			}
		}

		return Coefficient;
	}

	void BezierCurve(std::vector<Point2D>& samplePoint, const std::vector<Point2D>& ctrlPts)
	{
		samplePoint.clear();

		int nDdegree = ctrlPts.size();
		int* pCoefficient = CalcCoefficientMatrix(nDdegree);

		float* tVector = new float[nDdegree];
		float* tvMult = new float[nDdegree];

		for (int i = 0; i <= 100; i++)			
		{

			float t = 1.0f / 100.0f * i;
			for (int j = 0; j < nDdegree; j++)
			{

				tVector[j] = pow(t, j);
			}

			for (int k = 0; k < nDdegree; k++)
			{

				float dsum = 0.0;

				for (int j = 0; j < nDdegree; j++) 
				{ 
					float fVal = tVector[j] * pCoefficient[j*nDdegree + k];
					dsum += fVal;
				}

				tvMult[k] = dsum;
			}

			Point2D outPnt;
			for (int j = 0; j < nDdegree; j++)
			{
				outPnt.x += ctrlPts[j].x*tvMult[j];
				outPnt.y += ctrlPts[j].y*tvMult[j];	
			}

			samplePoint.push_back(outPnt);
		}	
	}


	void on_draw(HDC hdc)
	{
		agg::pixfmt_bgr24 pixf(m_renbuf);

		typedef agg::renderer_base<agg::pixfmt_bgr24> renderer_base_type;
		renderer_base_type renb(pixf);

		typedef agg::renderer_scanline_aa_solid<renderer_base_type> renderder_scanline_type;
		renderder_scanline_type rensl(renb);

		//clean srceen
		renb.clear(agg::rgba(1,1,1));

		agg::scanline_u8 sl;
		agg::rasterizer_scanline_aa<> ras;

		//--------------------------------------

		agg::path_storage ps;

		int iCount =m_samplePoints.size();
		if(iCount>0)
		{
			ps.move_to(m_samplePoints[0].x, m_samplePoints[0].y);
			for (int i = 1; i < iCount; i++)			
			{
				ps.line_to(m_samplePoints[i].x, m_samplePoints[i].y);
			}
		}
		
		int nCtrCnt = m_ctrlPoints.size();
		if(nCtrCnt>0)
		{
			ps.move_to(m_ctrlPoints[0].x, m_ctrlPoints[0].y);
			for (int i = 1; i < nCtrCnt; i++)			
			{
				ps.line_to(m_ctrlPoints[i].x, m_ctrlPoints[i].y);
			}
		}

		agg::conv_stroke<agg::path_storage> stroke(ps);

		stroke.width(2.5);
		ras.add_path(stroke);

		agg::render_scanlines_aa_solid(ras,sl,renb,agg::rgba8(85,85,85));
		ras.reset();

		//------------------------------------------------------
		for (int i = 0; i < m_ctrlPoints.size(); i++)
		{
			agg::ellipse ellipse(m_ctrlPoints[i].x, m_ctrlPoints[i].y,3,3); 

			ras.add_path(ellipse);
		}
		agg::render_scanlines_aa_solid(ras,sl,renb,agg::rgba8(85,85,85));
		ras.reset();
		
		m_pixel_map.blend(hdc,0,0);

	}

private:
	pixel_map              m_pixel_map;
	agg::rendering_buffer  m_renbuf;

	std::vector<Point2D>   m_samplePoints;
	std::vector<Point2D>   m_ctrlPoints;
	int                    m_ctrlIndex;
};
 
//******************************************************************************
 


 
//*******************************************************************************
int agg_main()
{
 	 the_application app;
 	 app.caption("cario Test");
 
 	 if(app.init(800,600, 0))
 	 {
 		 app.run();
 	 }
	 return 0;
}