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


class the_application : public  platform_support
{


public:
	the_application() 
	{

	}

	void on_init()
	{
		
	
     

		 
	}
	
	void on_resize(int sx, int sy)
	{
		m_pixel_map.create(sx, sy, pixel_map::org_color24);
	}

	void on_draw(HDC hdc)
	{
		agg::rendering_buffer renbuf;

		renbuf.attach(m_pixel_map.buf(), 
			m_pixel_map.width(),
			m_pixel_map.height(),
			m_flip_y ? m_pixel_map.stride() : -m_pixel_map.stride());


		typedef agg::renderer_base<agg::pixfmt_bgr24> ren_base;
		agg::pixfmt_bgr24 pixf(renbuf);
		ren_base ren(pixf);

		//clean srceen
		ren.clear(agg::rgba(1,1,1));

		//draw grcphics
		agg::scanline_u8 sl;



		m_pixel_map.blend(hdc,0,0);

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