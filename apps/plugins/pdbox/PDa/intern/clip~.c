#include "../src/m_pd.h"
#include "../src/m_fixed.h"

static t_class *clip_class;

typedef struct _clip
{
    t_object x_obj;
    float x_f;
    t_float x_lo;
    t_float x_hi;
} t_clip;

static void *clip_new(t_floatarg lo, t_floatarg hi)
{
    t_clip *x = (t_clip *)pd_new(clip_class);
    x->x_lo = lo;
    x->x_hi = hi;
    outlet_new(&x->x_obj, gensym("signal"));
    floatinlet_new(&x->x_obj, &x->x_lo);
    floatinlet_new(&x->x_obj, &x->x_hi);
    x->x_f = 0;
    return (x);
}

static t_int *clip_perform(t_int *w)
{
    t_clip *x = (t_clip *)(w[1]);
    t_sample *in = (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);
    int n = (int)(w[4]);
    t_sample lo;
    t_sample hi;
    lo = ftofix(x->x_lo);
    hi = ftofix(x->x_hi);

    while (n--)
    {
    	t_sample f = *in++;
	if (f < lo) f = lo;
	if (f > hi) f = hi;
    	*out++ = f;
    }
    return (w+5);
}

static void clip_dsp(t_clip *x, t_signal **sp)
{
    dsp_add(clip_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void clip_tilde_setup(void)
{
    clip_class = class_new(gensym("clip~"), (t_newmethod)clip_new, 0,
    	sizeof(t_clip), 0, A_DEFFLOAT, A_DEFFLOAT, 0);
    CLASS_MAINSIGNALIN(clip_class, t_clip, x_f);
    class_addmethod(clip_class, (t_method)clip_dsp, gensym("dsp"), 0);
}

