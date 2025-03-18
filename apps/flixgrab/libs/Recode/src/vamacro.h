/********************************************************************
    created:    2011/08/30
    created:    30:8:2011   17:40
    filename:   VariadicMacros.h
    file base:  VariadicMacros
    file ext:   h
    author:     fga
    
    purpose:    Variadic Macros Support
*********************************************************************/
#pragma once

#define VA_SIZE(...) INVOKE( VA_GET_SIZE VA_OB INVOKE(VA_SPEC##__VA_ARGS__()), 0, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,20,19,18,17,16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 VA_CB )

#define VA_OB (
#define VA_CB )
#define VA_SPEC() 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
#define VA_GET_SIZE(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_,n,...) n

#define INVOKE( ... ) INVOKE_A( __VA_ARGS__ )
#define INVOKE_A( ... ) __VA_ARGS__
#define VA_APPLY(x) x
#define CAT(x,y) CAT_A(x, y)
#define CAT_A(x,y) x##y

#define VA_PAIR(macro,...) INVOKE( CAT(VA_PAIR, VA_SIZE(__VA_ARGS__)) ( macro, (__VA_ARGS__) ) )

#define VA_FIRST_PAIR_TYPE( t,v,... ) t
#define VA_FIRST_PAIR_VAL( t,v,... ) v
#define VA_WO_FIRST_PAIR( t,v,... ) __VA_ARGS__

#define VA_PAIR0( m, x )
#define VA_PAIR2( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x ))
#define VA_PAIR4( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR2(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR6( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR4(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR8( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR6(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR10( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR8(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR12( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR10(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR14( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR12(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR16( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR14(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR18( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR16(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR20( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR18(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR22( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR20(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR24( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR22(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR26( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR24(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR28( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR26(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))
#define VA_PAIR30( m, x ) m( VA_APPLY( VA_FIRST_PAIR_TYPE x ), VA_APPLY( VA_FIRST_PAIR_VAL x )), VA_PAIR28(m, (VA_APPLY(VA_WO_FIRST_PAIR x)))

#define VA_VAL( t, v ) v
#define VA_ARG( t, v ) t v