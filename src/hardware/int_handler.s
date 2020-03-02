    .section text
    .extern irq1_handler

    .global irq1_handler
    .global irq1_caller
irq1_caller:    
    call irq1_handler
    iret
