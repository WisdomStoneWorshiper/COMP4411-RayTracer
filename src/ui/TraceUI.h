//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;

	Fl_Slider*			m_attConstSlider;
	Fl_Slider*			m_attLinearSlider;
	Fl_Slider*			m_attQuadSlider;
	Fl_Slider*			m_thresholdSlider;
	Fl_Slider*			m_subPixelSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;
	Fl_Check_Button*	m_superSamplingButton;
	Fl_Check_Button*	m_jitterButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();

	double getAttConst();
	double getAttLinear();
	double getAttQuad();
	double getThreshold();
	int getSubPixels();
	bool getSuperSampling();
	bool getJitter();

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;

	double m_nAttConst;
	double m_nAttLinear;
	double m_nAttQuad;
	double m_nThreshold;
	int m_nSubPixels;
	bool m_supersampling;
	bool m_jitter;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);

	static void cb_attConstSlides(Fl_Widget* o, void* v);
	static void cb_attLinearSlides(Fl_Widget* o, void* v);
	static void cb_attQuadSlides(Fl_Widget* o, void* v);
	static void cb_thresholdSlides(Fl_Widget* o, void* v);
	static void cb_subPixelSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
	static void cb_superSampling(Fl_Widget* o, void* v);
	static void cb_jitter(Fl_Widget* o, void* v);
};

#endif
