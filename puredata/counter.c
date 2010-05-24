#include "m_pd.h"
#include <unistd.h> //sleep()
#include <stdio.h> //printf()

static t_class *counter_class;

typedef struct _counter {
	t_object x_obj;
	t_outlet *x_out2;
	t_outlet *x_out3;
	t_int i_count;
	t_float step;
} t_counter;

void counter_bang(t_counter *x)
{
	x->i_count+=x->step;
	t_float f=x->i_count;
	t_float g=x->i_count*2;
	outlet_float(x->x_obj.ob_outlet, f);
	outlet_float(x->x_out2, g);
}

void counter_reset(t_counter *x)
{
	t_float f = x->i_count = 0;
	outlet_float(x->x_obj.ob_outlet, f);
}

void *counter_new(t_floatarg f)
{
	t_counter *x = (t_counter *)pd_new(counter_class);
	x->i_count=f;
	x->step = 1;
	outlet_new(&x->x_obj, &s_float);
	x->x_out2 = outlet_new(&x->x_obj, &s_float);
	x->x_out3 = outlet_new(&x->x_obj, &s_list);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("lol"), gensym("reset"));
	floatinlet_new(&x->x_obj, &x->step);
	return (void *)x;
}

void counter_setup(void) {
	counter_class = class_new(gensym("counter"),
		(t_newmethod)counter_new,
		0, sizeof(t_counter),
		CLASS_DEFAULT,
		A_DEFFLOAT, 0
	);
	class_addbang(counter_class, counter_bang);
	class_addmethod(counter_class, (t_method)counter_reset, gensym("reset"), 0);
}

