//Dynamic loading of modules into the kernel.
#include <linux/module.h>

//Needed for kernal info .
#include <linux/kernel.h>

//API to acheive driver logic using USB horizontal.
#include <linux/usb.h>

//Needed for using the macros.
#include <linux/init.h>

/* 

    struct usb_interface : 
    DESCRIPTION : 
        USB interfaces are described in the kernel with the struct usb_interface structure.
        This structure is what the USB core passes to USB drivers and is what the USB driver
        then is in charge of controlling.

    struct usb_device_id 
    DESCRIPTION : 
        The structure provides a list of different types of USB devices that this driver   
        supports.This list is used by the USB core to decide which driver to
        give a device to, and by the hotplug scripts to decide which driver to automatically
        load when a specific device is plugged into the system.

*/

//Pen Probe is a function for initializing the Driver
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    /* 
        printk : 
            printk is a function that prints messages and is used in the C Programming Language 
            exclusively for the Linux Kernel. It accepts a string parameter called the format 
            string, which specifies a method for rendering an arbitrary number of varied data 
            type parameter(s) into a string. The string is then printed to the kernel log.
        
    */
    printk(KERN_INFO "Pen drive (%04X:%04X) plugged \n", id->idVendor, id->idProduct);
    return 0;
}

//Function to notify the removal of USB 
static void pen_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen drive removed \n");
}

//This function stores array of usb_device_id structure 
static struct usb_device_id pen_table[] =
{
    // USB_DEVICE(USB_SKEL_VENDOR_ID, USB_SKEL_PRODUCT_ID)
    /*
        USB_DEVICE(vendor, product)
            Creates a struct usb_device_id that can be used to match only the specified vendor
            and product ID values. This is very commonly used for USB devices that
            need a specific driver.
        USB_SKEL_VENDOR_ID :
            The USB vendor ID for the device. This number is assigned by the USB forum to
            its members and cannot be made up by anyone else.

        USB_SKEL_PRODUCT_ID :
            The USB product ID for the device. All vendors that have a vendor ID assigned
            to them can manage their product IDs however they choose to.
    */

    { USB_DEVICE(0x17ef,0x602e) },
     { USB_DEVICE(0x054c,0x05ba) },
    {} // Terminating entry

};

/* 
    As with a PCI driver, the MODULE_DEVICE_TABLE macro is necessary to allow user-space
    tools to figure out what devices this driver can control. But for USB drivers, 
    the string "usb" must be the first value in the macro.

    PCI (Peripheral Component Interconnect) :
    It is a local computer bus for attaching hardware devices in a computer.

*/
MODULE_DEVICE_TABLE (usb, pen_table);
 
//The structure is used for the registration of the device driver
static struct usb_driver pen_driver =
{
    /*  
        const char *name :
            Pointer to the name of the driver. It must be unique amongall USB drivers in the
            kernel and is normally set to the same name as the module name of the driver. It
            shows up in sysfs under /sys/bus/usb/drivers/ when the driver is in the kernel.
    */
    .name = "pen_driver",

    /*
        const struct usb_device_id *id_table :
            Pointer to the struct usb_device_id table that contains a list of all of the different
            kinds of USB devices this driver can accept. If this variable is not set, the
            probe function callback in the USB driver is never called.

    */
    .id_table = pen_table,
    
    /*
        int (*probe) (struct usb_interface *intf, const struct usb_device_id *id) :
            Pointer to the probe function in the USB driver.This functionis called by the USB core 
            when it thinks it has a struct usb_interface that this driver can handle.
            A pointer to the struct usb_device_id that the USB core used to make this decision is also 
            passed to this function. If the USB driver claims the struct usb_interface that is passed
            to it, it should initialize the device properly and return 0. If the driver does not
            want to claim the device, or an error occurs, it should return a negative error value.
    */
    .probe = pen_probe,

    /*
        void (*disconnect) (struct usb_interface *intf) :
            Pointer to the disconnect function in the USB driver. This function is called by the USB core 
            when the struct usb_interface has been removed from the system or when the driver is
            being unloaded from the USB core.
    */
    .disconnect = pen_disconnect,

};

/*
    The function helps for the initializing of the driver.
    It returns the pointer to the structure that contains definition of the driver .

*/
static int __init pen_init(void)
{

    return usb_register(&pen_driver);

}
 
/*
    The function helps for the disconnection of the driver.
    It returns the pointer to the structure that contains definition of the driver .   
*/
static void __exit pen_exit(void)
{

    usb_deregister(&pen_driver);

}

/*
    Using the module_init() and module_exit() macros it is easy to write code without 
    #ifdefs which can operate both as a module or built into the kernel.

    module_init():
        The module_init() macro defines which function is to be called at module insertion 
        time (if the file is compiled as a module), or at boot time.
    
    module_exit() :
        The module_exit macro defines the function to be called at module removal time 
        (or never, in the case of the file compiled into the kernel).
*/
module_init(pen_init);
module_exit(pen_exit);
 
//MODULE_LICENSE() is a macro which allows loadable kernel modules to declare their license to the world.
MODULE_LICENSE("GPL"); // GPL : General Public License

//MODULE_AUTHOR() is a macro which helps to assign a author to the module 
MODULE_AUTHOR("Code busters");

//MODULE_DESCRIPTION() is a macro which assigns a desription about what our module does.
MODULE_DESCRIPTION("USB Pen Registration Driver");
