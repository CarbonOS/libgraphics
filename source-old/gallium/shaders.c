/* Static Device Shader Source */

/* Static Vertex Shader */
static const char *vs_text =
    "VERT\n"
    "DCL IN[0]\n"
    "DCL IN[1]\n"
    "DCL OUT[0], POSITION\n"
    "DCL OUT[1], GENERIC[0]\n"
    "DCL CONST[0], CONSTANT\n" // Viewport
    "DCL CONST[1], CONSTANT\n" // Transform
    "DCL CONST[2], CONSTANT\n" // Transform
    "DCL CONST[3], CONSTANT\n" // Transform
    "DCL CONST[4], CONSTANT\n" // Transform
    "DCL TEMP[0..4]\n"
    "  0: MUL TEMP[1], IN[0].xxxx, CONST[1]\n"
    "  1: MUL TEMP[2], IN[0].yyyy, CONST[2]\n"
    "  2: ADD TEMP[1], TEMP[1], TEMP[2]\n"
    "  3: ADD TEMP[1], TEMP[1], CONST[4]\n"
    "  4: DIV TEMP[0], TEMP[1], CONST[0]\n"
    "  5: MOV OUT[0], TEMP[0]\n"
    "  6: MOV OUT[1], IN[1]\n"
    "  7: END\n";

/* Creates the Static Vertex Shader State for the Device. */
static int galCreateVertexShader(GIDevice *idevice)
{
	void *handle;
	struct tgsi_token tokens[64];
	struct pipe_shader_state vs_state;
	memset(&vs_state, 0, sizeof(vs_state));

	idevice->vs_state = NULL;
	if (!tgsi_text_translate(vs_text, tokens, Elements(tokens)))
	{
		printf("tgsi_text_translate failed!\n");
		return GI_ERROR;
	}

	vs_state.tokens = tokens;
	if ((handle = idevice->context->create_vs_state(idevice->context, &vs_state)) == NULL)
	{
		printf("create_vs_state failed!\n");
		return GI_ERROR;
	}

	SetVertexShader(idevice->context, handle);
	idevice->vs_state = handle;

	return GI_SUCCESS;
}

/* Static Color Pixel Shader */
static const char *ps_color =
    "FRAG\n"
    "DCL IN[0], POSITION, LINEAR\n"
    "DCL IN[1], GENERIC[0], LINEAR\n"
    "DCL OUT[0], COLOR\n"
    "DCL TEMP[0]\n"
    "  0: MOV OUT[0], IN[1]\n"
    "  1: END\n";

/* Creates the Static Pixel Shader State for the Device. */
static inline int galCreateColorPixelShader(GIDevice *idevice)
{
	void *handle;
	struct tgsi_token tokens[64];
	struct pipe_shader_state fs_state;
	memset(&fs_state, 0, sizeof(fs_state));

	idevice->fs_color = NULL;
	if (!tgsi_text_translate(ps_color, tokens, Elements(tokens)))
	{
		printf("tgsi_text_translate failed!\n");
		return GI_ERROR;
	}

	fs_state.tokens = tokens;
	if ((handle = idevice->context->create_fs_state(idevice->context, &fs_state)) == NULL)
	{
		printf("create_fs_state failed!\n");
		return GI_ERROR;
	}

	idevice->fs_color = handle;
	return GI_SUCCESS;
}

/* Static Image Pixel Shader */
static const char *ps_image =
    "FRAG\n"
    "DCL IN[0], POSITION, LINEAR\n"
    "DCL IN[1], GENERIC[0], LINEAR\n"
    "DCL OUT[0], COLOR\n"
    "DCL SAMP[0]\n"
    "DCL TEMP[0]\n"
    "  0: TEX TEMP[0], IN[1], SAMP[0], 2D\n"
    "  1: MOV OUT[0], TEMP[0]\n"
    "  2: END\n";

/* Creates the Static Pixel Shader State for the Device. */
static inline int galCreateImagePixelShader(GIDevice *idevice)
{
	void *handle;
	struct tgsi_token tokens[64];
	struct pipe_shader_state fs_state;
	memset(&fs_state, 0, sizeof(fs_state));

	idevice->fs_image = NULL;
	if (!tgsi_text_translate(ps_image, tokens, Elements(tokens)))
	{
		printf("tgsi_text_translate failed!\n");
		return GI_ERROR;
	}

	fs_state.tokens = tokens;
	if ((handle = idevice->context->create_fs_state(idevice->context, &fs_state)) == NULL)
	{
		printf("create_fs_state failed!\n");
		return GI_ERROR;
	}

	idevice->fs_image = handle;
	return GI_SUCCESS;
}

