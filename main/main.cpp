#include <vmchset.h>
#include "vmsys.h"
#include "vmsrvmng.h"
#include <vmtimer.h>

#define IBase GS_IBase

#include "gs_id.h"
#include "srv_imageviewer_interface.h"

void timer1(int a) {
    vm_delete_timer_ex(a);
    vm_exit_app();
}

static void handle_system_event(VMINT message, VMINT param)
{
    if (message == VM_MSG_CREATE)
    {
        IImageviewer *viewer = NULL;

        VMINT ret = vm_create_service(SID_IMAGEVIEWERSERVICE,IID_IMAGEVIEWER, (void**)&viewer);

        if (ret >= 0 && viewer)
        {
            IImageviewer_create(viewer);

            VMWCHAR wpath[260];

            vm_ascii_to_ucs2(wpath, sizeof(wpath), "e:\\demo.jpg");

            IImageviewer_setModeFileName(viewer, (PS8)wpath);

            IImageviewer_run(viewer);

            IImageviewer_release(viewer);
        }

        vm_create_timer_ex(10, timer1);
    }
}

void vm_main(void)
{
    vm_reg_sysevt_callback(handle_system_event);
}
