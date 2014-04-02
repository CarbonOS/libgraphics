/* Software Effect */

static COLOR* ExecuteEffectSP(IEffectSP *effect, COLOR *in)
{
	return in;
}

/* Creates a new Software Effect object. */
static IEffect* CreateEffectSP(IExtension *ext)
{
	IEffectSP *effect = NULL;
	if (!(effect = (IEffectSP*)calloc(1, sizeof(IEffectSP))))
	{
		return NULL;
	}

	effect->base.ext = ext;
	return (&effect->base);
}

/* Destroys a Software Context object. */
static int ReleaseEffectSP(IEffect *effect)
{
	IEffectSP *effectsp = NULL;
	if (!(effectsp = (IEffectSP*)effect))
	{
		return 0;
	}

	free(effectsp);
	return 1;
}

