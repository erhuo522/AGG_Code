#include <windows.h>
 
#include "agg_platform_support.h"
#include "agg_win32_bmp.h" 

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_scanline_u.h"

class the_application : public  platform_support
{


public:
	the_application() 
	{

	}

	void on_init()
	{
		

		 
	}

	void on_draw(HDC hdc)
	{
		/*
		 typedef agg::renderer_base<agg::pixfmt_bgr24> ren_base;

		 agg::pixfmt_bgr24 pixf(m_pixel_map.buf());
        ren_base ren(pixf);
        agg::scanline_u8 sl;

		ren.clear(agg::rgba(1,1,1));


		m_pixel_map.blend(hdc,0,0);
		*/
	}

private:
	pixel_map  m_pixel_map;
	bool      m_flip_y;


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