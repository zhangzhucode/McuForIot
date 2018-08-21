# mcuLeds
    基于stm32f103c8t6的工程

src 结构：
    1 stm32dev、stm32lib 这里是stm32 cpu的基础设置和固件库
    2 dev 这里是一些外设
    3 system 这里是独立于外设的代码，里面包含纯软件实现的逻辑，或者对cpu封装的接口
    4 demo 开头的都是具体的上层app code和编译代码
    
    
