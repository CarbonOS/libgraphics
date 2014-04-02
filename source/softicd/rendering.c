/* Rendering Tasks */

static int RenderFunc(void* arg)
{
	TASKMSG msg;
	printf("task ...\n");
	while (GetTaskMessage(&msg, 0))
	{
		printf("msg: %d\n", msg.type);
		printf("%d\n", msg.param1);
	}

	return 0;
}

/* Software FillRectangle */
static int RenderingInit(IContextSP *context)
{
	int index;
/*
	for (index = 0; index < context->threads; index++)
	{
		if (!(context->task[index] = CreateTask(RenderFunc, NULL)))
		{
			printf("CreateTask() failed!\n");
			return 0;
		}
	}
*/
	return 1;
}

