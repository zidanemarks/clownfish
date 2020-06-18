#include "clownfish_common_mem.cc"

template<uint32_t dsize, uint_t asize, uint32_t msize> 
void RFRAM<dsize, asize, msize>::MemoryInit(){
    if(memory !=NULL)
        free(memory);
    memory = (char *)malloc(sizof(char)*msize);
    memset(memory, 0, msize);
} 

template<uint32_t dsize, uint32_t asize, uint32_t msize> 
void RFRAM<dsize, asize, msize>::MemoryAccess(){
    if(!reset_n){
        if(!ce_n){
            //read
            if(we_n)
                for(uint32_t i=0; i<dsize/8; i++)
                    rdata_o.range(8*(i+1), 8*i) = *(memory+address_i+i);   
            //write
            else
                for(uint32_t i=0; i<dsize/8; i++)
                    *(memory+address_i+i) = wdata_i.range(8*(i+1), 8*i);   

        }
    }
}

