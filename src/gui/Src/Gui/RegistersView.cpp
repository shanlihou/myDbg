#include <QClipboard>
#include <QMessageBox>
#include <QListWidget>
#include <stdint.h>
#include "RegistersView.h"
#include "Configuration.h"
#include "WordEditDialog.h"
#include "LineEditDialog.h"
#include "EditFloatRegister.h"
#include "SelectFields.h"
#include "MiscUtil.h"

int RegistersView::getEstimateHeight()
{
    return mRowsNeeded * mRowHeight;
}

void RegistersView::SetChangeButton(QPushButton* push_button)
{
    mChangeViewButton = push_button;
    fontsUpdatedSlot();
}

void RegistersView::InitMappings()
{
    // create mapping from internal id to name
    mRegisterMapping.clear();
    mRegisterPlaces.clear();
    int offset = 0;

    /* Register_Position is a struct definition the position
     *
     * (line , start, labelwidth, valuesize )
     */
#ifdef _WIN64
    mRegisterMapping.insert(CAX, "RAX");
    mRegisterPlaces.insert(CAX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CBX, "RBX");
    mRegisterPlaces.insert(CBX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CCX, "RCX");
    mRegisterPlaces.insert(CCX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CDX, "RDX");
    mRegisterPlaces.insert(CDX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CBP, "RBP");
    mRegisterPlaces.insert(CBP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CSP, "RSP");
    mRegisterPlaces.insert(CSP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CSI, "RSI");
    mRegisterPlaces.insert(CSI, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CDI, "RDI");
    mRegisterPlaces.insert(CDI, Register_Position(offset++, 0, 6, sizeof(duint) * 2));

    offset++;

    mRegisterMapping.insert(R8, "R8");
    mRegisterPlaces.insert(R8 , Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R9, "R9");
    mRegisterPlaces.insert(R9 , Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R10, "R10");
    mRegisterPlaces.insert(R10, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R11, "R11");
    mRegisterPlaces.insert(R11, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R12, "R12");
    mRegisterPlaces.insert(R12, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R13, "R13");
    mRegisterPlaces.insert(R13, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R14, "R14");
    mRegisterPlaces.insert(R14, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(R15, "R15");
    mRegisterPlaces.insert(R15, Register_Position(offset++, 0, 6, sizeof(duint) * 2));

    offset++;

    mRegisterMapping.insert(CIP, "RIP");
    mRegisterPlaces.insert(CIP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));

    offset++;

    mRegisterMapping.insert(EFLAGS, "RFLAGS");
    mRegisterPlaces.insert(EFLAGS, Register_Position(offset++, 0, 9, sizeof(duint) * 2));
#else //x32
    mRegisterMapping.insert(CAX, "EAX");
    mRegisterPlaces.insert(CAX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CBX, "EBX");
    mRegisterPlaces.insert(CBX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CCX, "ECX");
    mRegisterPlaces.insert(CCX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CDX, "EDX");
    mRegisterPlaces.insert(CDX, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CBP, "EBP");
    mRegisterPlaces.insert(CBP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CSP, "ESP");
    mRegisterPlaces.insert(CSP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CSI, "ESI");
    mRegisterPlaces.insert(CSI, Register_Position(offset++, 0, 6, sizeof(duint) * 2));
    mRegisterMapping.insert(CDI, "EDI");
    mRegisterPlaces.insert(CDI, Register_Position(offset++, 0, 6, sizeof(duint) * 2));

    offset++;

    mRegisterMapping.insert(CIP, "EIP");
    mRegisterPlaces.insert(CIP, Register_Position(offset++, 0, 6, sizeof(duint) * 2));

    offset++;

    mRegisterMapping.insert(EFLAGS, "EFLAGS");
    mRegisterPlaces.insert(EFLAGS, Register_Position(offset++, 0, 9, sizeof(duint) * 2));
#endif

    mRegisterMapping.insert(ZF, "ZF");
    mRegisterPlaces.insert(ZF, Register_Position(offset, 0, 3, 1));
    mRegisterMapping.insert(PF, "PF");
    mRegisterPlaces.insert(PF, Register_Position(offset, 6, 3, 1));
    mRegisterMapping.insert(AF, "AF");
    mRegisterPlaces.insert(AF, Register_Position(offset++, 12, 3, 1));

    mRegisterMapping.insert(OF, "OF");
    mRegisterPlaces.insert(OF, Register_Position(offset, 0, 3, 1));
    mRegisterMapping.insert(SF, "SF");
    mRegisterPlaces.insert(SF, Register_Position(offset, 6, 3, 1));
    mRegisterMapping.insert(DF, "DF");
    mRegisterPlaces.insert(DF, Register_Position(offset++, 12, 3, 1));

    mRegisterMapping.insert(CF, "CF");
    mRegisterPlaces.insert(CF, Register_Position(offset, 0, 3, 1));
    mRegisterMapping.insert(TF, "TF");
    mRegisterPlaces.insert(TF, Register_Position(offset, 6, 3, 1));
    mRegisterMapping.insert(IF, "IF");
    mRegisterPlaces.insert(IF, Register_Position(offset++, 12, 3, 1));

    offset++;

    mRegisterMapping.insert(LastError, "LastError");
    mRegisterPlaces.insert(LastError, Register_Position(offset++, 0, 10, 20));

    offset++;

    mRegisterMapping.insert(GS, "GS");
    mRegisterPlaces.insert(GS, Register_Position(offset, 0, 3, 4));
    mRegisterMapping.insert(FS, "FS");
    mRegisterPlaces.insert(FS, Register_Position(offset++, 9, 3, 4));
    mRegisterMapping.insert(ES, "ES");
    mRegisterPlaces.insert(ES, Register_Position(offset, 0, 3, 4));
    mRegisterMapping.insert(DS, "DS");
    mRegisterPlaces.insert(DS, Register_Position(offset++, 9, 3, 4));
    mRegisterMapping.insert(CS, "CS");
    mRegisterPlaces.insert(CS, Register_Position(offset, 0, 3, 4));
    mRegisterMapping.insert(SS, "SS");
    mRegisterPlaces.insert(SS, Register_Position(offset++, 9, 3, 4));

    if(mShowFpu)
    {
        offset++;

        mRegisterMapping.insert(x87r0, "x87r0");
        mRegisterPlaces.insert(x87r0, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r1, "x87r1");
        mRegisterPlaces.insert(x87r1, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r2, "x87r2");
        mRegisterPlaces.insert(x87r2, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r3, "x87r3");
        mRegisterPlaces.insert(x87r3, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r4, "x87r4");
        mRegisterPlaces.insert(x87r4, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r5, "x87r5");
        mRegisterPlaces.insert(x87r5, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r6, "x87r6");
        mRegisterPlaces.insert(x87r6, Register_Position(offset++, 0, 6, 10 * 2));
        mRegisterMapping.insert(x87r7, "x87r7");
        mRegisterPlaces.insert(x87r7, Register_Position(offset++, 0, 6, 10 * 2));

        offset++;

        mRegisterMapping.insert(x87TagWord, "x87TagWord");
        mRegisterPlaces.insert(x87TagWord, Register_Position(offset++, 0, 11, sizeof(WORD) * 2));

        mRegisterMapping.insert(x87TW_0, "x87TW_0");
        mRegisterPlaces.insert(x87TW_0, Register_Position(offset, 0, 8, 10));
        mRegisterMapping.insert(x87TW_1, "x87TW_1");
        mRegisterPlaces.insert(x87TW_1, Register_Position(offset++, 20, 8, 10));

        mRegisterMapping.insert(x87TW_2, "x87TW_2");
        mRegisterPlaces.insert(x87TW_2, Register_Position(offset, 0, 8, 10));
        mRegisterMapping.insert(x87TW_3, "x87TW_3");
        mRegisterPlaces.insert(x87TW_3, Register_Position(offset++, 20, 8, 10));

        mRegisterMapping.insert(x87TW_4, "x87TW_4");
        mRegisterPlaces.insert(x87TW_4, Register_Position(offset, 0, 8, 10));
        mRegisterMapping.insert(x87TW_5, "x87TW_5");
        mRegisterPlaces.insert(x87TW_5, Register_Position(offset++, 20, 8, 10));

        mRegisterMapping.insert(x87TW_6, "x87TW_6");
        mRegisterPlaces.insert(x87TW_6, Register_Position(offset, 0, 8, 10));
        mRegisterMapping.insert(x87TW_7, "x87TW_7");
        mRegisterPlaces.insert(x87TW_7, Register_Position(offset++, 20, 8, 10));

        offset++;

        mRegisterMapping.insert(x87StatusWord, "x87StatusWord");
        mRegisterPlaces.insert(x87StatusWord, Register_Position(offset++, 0, 14, sizeof(WORD) * 2));

        mRegisterMapping.insert(x87SW_B, "x87SW_B");
        mRegisterPlaces.insert(x87SW_B, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87SW_C3, "x87SW_C3");
        mRegisterPlaces.insert(x87SW_C3, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87SW_C2, "x87SW_C2");
        mRegisterPlaces.insert(x87SW_C2, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87SW_C1, "x87SW_C1");
        mRegisterPlaces.insert(x87SW_C1, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87SW_C0, "x87SW_C0");
        mRegisterPlaces.insert(x87SW_C0, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87SW_IR, "x87SW_IR");
        mRegisterPlaces.insert(x87SW_IR, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87SW_SF, "x87SW_SF");
        mRegisterPlaces.insert(x87SW_SF, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87SW_P, "x87SW_P");
        mRegisterPlaces.insert(x87SW_P, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87SW_U, "x87SW_U");
        mRegisterPlaces.insert(x87SW_U, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87SW_O, "x87SW_O");
        mRegisterPlaces.insert(x87SW_O, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87SW_Z, "x87SW_Z");
        mRegisterPlaces.insert(x87SW_Z, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87SW_D, "x87SW_D");
        mRegisterPlaces.insert(x87SW_D, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87SW_I, "x87SW_I");
        mRegisterPlaces.insert(x87SW_I, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87SW_TOP, "x87SW_TOP");
        mRegisterPlaces.insert(x87SW_TOP, Register_Position(offset++, 12, 10, 13));

        offset++;

        mRegisterMapping.insert(x87ControlWord, "x87ControlWord");
        mRegisterPlaces.insert(x87ControlWord, Register_Position(offset++, 0, 15, sizeof(WORD) * 2));

        mRegisterMapping.insert(x87CW_IC, "x87CW_IC");
        mRegisterPlaces.insert(x87CW_IC, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87CW_IEM, "x87CW_IEM");
        mRegisterPlaces.insert(x87CW_IEM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87CW_PM, "x87CW_PM");
        mRegisterPlaces.insert(x87CW_PM, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87CW_UM, "x87CW_UM");
        mRegisterPlaces.insert(x87CW_UM, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87CW_OM, "x87CW_OM");
        mRegisterPlaces.insert(x87CW_OM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87CW_ZM, "x87CW_ZM");
        mRegisterPlaces.insert(x87CW_ZM, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(x87CW_DM, "x87CW_DM");
        mRegisterPlaces.insert(x87CW_DM, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(x87CW_IM, "x87CW_IM");
        mRegisterPlaces.insert(x87CW_IM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(x87CW_RC, "x87CW_RC");
        mRegisterPlaces.insert(x87CW_RC, Register_Position(offset++, 25, 10, 14));

        mRegisterMapping.insert(x87CW_PC, "x87CW_PC");
        mRegisterPlaces.insert(x87CW_PC, Register_Position(offset++, 0, 9, 14));

        offset++;

        mRegisterMapping.insert(MxCsr, "MxCsr");
        mRegisterPlaces.insert(MxCsr, Register_Position(offset++, 0, 6, sizeof(DWORD) * 2));

        mRegisterMapping.insert(MxCsr_FZ, "MxCsr_FZ");
        mRegisterPlaces.insert(MxCsr_FZ, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(MxCsr_PM, "MxCsr_PM");
        mRegisterPlaces.insert(MxCsr_PM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(MxCsr_UM, "MxCsr_UM");
        mRegisterPlaces.insert(MxCsr_UM, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(MxCsr_OM, "MxCsr_OM");
        mRegisterPlaces.insert(MxCsr_OM, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(MxCsr_ZM, "MxCsr_ZM");
        mRegisterPlaces.insert(MxCsr_ZM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(MxCsr_IM, "MxCsr_IM");
        mRegisterPlaces.insert(MxCsr_IM, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(MxCsr_UE, "MxCsr_UE");
        mRegisterPlaces.insert(MxCsr_UE, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(MxCsr_PE, "MxCsr_PE");
        mRegisterPlaces.insert(MxCsr_PE, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(MxCsr_DAZ, "MxCsr_DAZ");
        mRegisterPlaces.insert(MxCsr_DAZ, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(MxCsr_OE, "MxCsr_OE");
        mRegisterPlaces.insert(MxCsr_OE, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(MxCsr_ZE, "MxCsr_ZE");
        mRegisterPlaces.insert(MxCsr_ZE, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(MxCsr_DE, "MxCsr_DE");
        mRegisterPlaces.insert(MxCsr_DE, Register_Position(offset++, 25, 10, 1));

        mRegisterMapping.insert(MxCsr_IE, "MxCsr_IE");
        mRegisterPlaces.insert(MxCsr_IE, Register_Position(offset, 0, 9, 1));
        mRegisterMapping.insert(MxCsr_DM, "MxCsr_DM");
        mRegisterPlaces.insert(MxCsr_DM, Register_Position(offset, 12, 10, 1));
        mRegisterMapping.insert(MxCsr_RC, "MxCsr_RC");
        mRegisterPlaces.insert(MxCsr_RC, Register_Position(offset++, 25, 10, 19));

        offset++;

        mRegisterMapping.insert(MM0, "MM0");
        mRegisterPlaces.insert(MM0, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM1, "MM1");
        mRegisterPlaces.insert(MM1, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM2, "MM2");
        mRegisterPlaces.insert(MM2, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM3, "MM3");
        mRegisterPlaces.insert(MM3, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM4, "MM4");
        mRegisterPlaces.insert(MM4, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM5, "MM5");
        mRegisterPlaces.insert(MM5, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM6, "MM6");
        mRegisterPlaces.insert(MM6, Register_Position(offset++, 0, 4, 8 * 2));
        mRegisterMapping.insert(MM7, "MM7");
        mRegisterPlaces.insert(MM7, Register_Position(offset++, 0, 4, 8 * 2));

        offset++;

        mRegisterMapping.insert(XMM0, "XMM0");
        mRegisterPlaces.insert(XMM0, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM1, "XMM1");
        mRegisterPlaces.insert(XMM1, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM2, "XMM2");
        mRegisterPlaces.insert(XMM2, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM3, "XMM3");
        mRegisterPlaces.insert(XMM3, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM4, "XMM4");
        mRegisterPlaces.insert(XMM4, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM5, "XMM5");
        mRegisterPlaces.insert(XMM5, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM6, "XMM6");
        mRegisterPlaces.insert(XMM6, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM7, "XMM7");
        mRegisterPlaces.insert(XMM7, Register_Position(offset++, 0, 6, 16 * 2));
#ifdef _WIN64
        mRegisterMapping.insert(XMM8, "XMM8");
        mRegisterPlaces.insert(XMM8, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM9, "XMM9");
        mRegisterPlaces.insert(XMM9, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM10, "XMM10");
        mRegisterPlaces.insert(XMM10, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM11, "XMM11");
        mRegisterPlaces.insert(XMM11, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM12, "XMM12");
        mRegisterPlaces.insert(XMM12, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM13, "XMM13");
        mRegisterPlaces.insert(XMM13, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM14, "XMM14");
        mRegisterPlaces.insert(XMM14, Register_Position(offset++, 0, 6, 16 * 2));
        mRegisterMapping.insert(XMM15, "XMM15");
        mRegisterPlaces.insert(XMM15, Register_Position(offset++, 0, 6, 16 * 2));
#endif

        offset++;

        mRegisterMapping.insert(YMM0, "YMM0");
        mRegisterPlaces.insert(YMM0, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM1, "YMM1");
        mRegisterPlaces.insert(YMM1, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM2, "YMM2");
        mRegisterPlaces.insert(YMM2, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM3, "YMM3");
        mRegisterPlaces.insert(YMM3, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM4, "YMM4");
        mRegisterPlaces.insert(YMM4, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM5, "YMM5");
        mRegisterPlaces.insert(YMM5, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM6, "YMM6");
        mRegisterPlaces.insert(YMM6, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM7, "YMM7");
        mRegisterPlaces.insert(YMM7, Register_Position(offset++, 0, 6, 32 * 2));
#ifdef _WIN64
        mRegisterMapping.insert(YMM8, "YMM8");
        mRegisterPlaces.insert(YMM8, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM9, "YMM9");
        mRegisterPlaces.insert(YMM9, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM10, "YMM10");
        mRegisterPlaces.insert(YMM10, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM11, "YMM11");
        mRegisterPlaces.insert(YMM11, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM12, "YMM12");
        mRegisterPlaces.insert(YMM12, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM13, "YMM13");
        mRegisterPlaces.insert(YMM13, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM14, "YMM14");
        mRegisterPlaces.insert(YMM14, Register_Position(offset++, 0, 6, 32 * 2));
        mRegisterMapping.insert(YMM15, "YMM15");
        mRegisterPlaces.insert(YMM15, Register_Position(offset++, 0, 6, 32 * 2));
#endif
    }

    offset++;

    mRegisterMapping.insert(DR0, "DR0");
    mRegisterPlaces.insert(DR0, Register_Position(offset++, 0, 4, sizeof(duint) * 2));
    mRegisterMapping.insert(DR1, "DR1");
    mRegisterPlaces.insert(DR1, Register_Position(offset++, 0, 4, sizeof(duint) * 2));
    mRegisterMapping.insert(DR2, "DR2");
    mRegisterPlaces.insert(DR2, Register_Position(offset++, 0, 4, sizeof(duint) * 2));
    mRegisterMapping.insert(DR3, "DR3");
    mRegisterPlaces.insert(DR3, Register_Position(offset++, 0, 4, sizeof(duint) * 2));
    mRegisterMapping.insert(DR6, "DR6");
    mRegisterPlaces.insert(DR6, Register_Position(offset++, 0, 4, sizeof(duint) * 2));
    mRegisterMapping.insert(DR7, "DR7");
    mRegisterPlaces.insert(DR7, Register_Position(offset++, 0, 4, sizeof(duint) * 2));

    mRowsNeeded = offset + 1;
}

RegistersView::RegistersView(QWidget* parent) : QScrollArea(parent), mVScrollOffset(0)
{
    mChangeViewButton = NULL;

    // precreate ContextMenu Actions
    wCM_Increment = new QAction(DIcon("register_inc.png"), tr("Increment"), this);
    wCM_Increment->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_Increment);
    wCM_Decrement = new QAction(DIcon("register_dec.png"), tr("Decrement"), this);
    wCM_Decrement->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_Decrement);
    wCM_Zero = new QAction(DIcon("register_zero.png"), tr("Zero"), this);
    wCM_Zero->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_Zero);
    wCM_SetToOne = new QAction(DIcon("register_one.png"), tr("Set to 1"), this);
    wCM_SetToOne->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_SetToOne);
    wCM_Modify = new QAction(DIcon("register_edit.png"), tr("Modify value"), this);
    wCM_Modify->setShortcut(QKeySequence(Qt::Key_Enter));
    wCM_ToggleValue = new QAction(DIcon("register_toggle.png"), tr("Toggle"), this);
    wCM_ToggleValue->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_ToggleValue);
    wCM_CopyToClipboard = new QAction(DIcon("copy.png"), tr("Copy value to clipboard"), this);
    wCM_CopyToClipboard->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_CopyToClipboard);
    wCM_CopySymbolToClipboard = new QAction(DIcon("pdb.png"), tr("Copy Symbol Value to Clipboard"), this);
    wCM_CopySymbolToClipboard->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_CopySymbolToClipboard);
    wCM_CopyAll = new QAction(DIcon("copy-alt.png"), tr("Copy all registers"), this);
    wCM_CopyAll->setShortcutContext(Qt::WidgetShortcut);
    this->addAction(wCM_CopyAll);
    wCM_FollowInDisassembly = new QAction(DIcon(QString("processor%1.png").arg(ArchValue("32", "64"))), tr("Follow in Disassembler"), this);
    wCM_FollowInDump = new QAction(DIcon("dump.png"), tr("Follow in Dump"), this);
    wCM_FollowInStack = new QAction(DIcon("stack.png"), tr("Follow in Stack"), this);
    wCM_Incrementx87Stack = new QAction(DIcon("arrow-small-down.png"), tr("Increment x87 Stack"), this);
    wCM_Decrementx87Stack = new QAction(DIcon("arrow-small-up.png"), tr("Decrement x87 Stack"), this);
    wCM_ChangeFPUView = new QAction(DIcon("change-view.png"), tr("Change view"), this);
    wCM_IncrementPtrSize = new QAction(ArchValue(tr("Increase 4"), tr("Increase 8")), this);
    wCM_DecrementPtrSize = new QAction(ArchValue(tr("Decrease 4"), tr("Decrease 8")), this);
    wCM_Push = new QAction(tr("Push"), this);
    wCM_Pop = new QAction(tr("Pop"), this);

    // general purposes register (we allow the user to modify the value)
    mGPR.insert(CAX);
    mCANSTOREADDRESS.insert(CAX);
    mUINTDISPLAY.insert(CAX);
    mLABELDISPLAY.insert(CAX);
    mMODIFYDISPLAY.insert(CAX);
    mINCREMENTDECREMET.insert(CAX);
    mSETONEZEROTOGGLE.insert(CAX);

    mSETONEZEROTOGGLE.insert(CBX);
    mINCREMENTDECREMET.insert(CBX);
    mGPR.insert(CBX);
    mUINTDISPLAY.insert(CBX);
    mLABELDISPLAY.insert(CBX);
    mMODIFYDISPLAY.insert(CBX);
    mCANSTOREADDRESS.insert(CBX);

    mSETONEZEROTOGGLE.insert(CCX);
    mINCREMENTDECREMET.insert(CCX);
    mGPR.insert(CCX);
    mUINTDISPLAY.insert(CCX);
    mLABELDISPLAY.insert(CCX);
    mMODIFYDISPLAY.insert(CCX);
    mCANSTOREADDRESS.insert(CCX);

    mSETONEZEROTOGGLE.insert(CDX);
    mINCREMENTDECREMET.insert(CDX);
    mGPR.insert(CDX);
    mUINTDISPLAY.insert(CDX);
    mLABELDISPLAY.insert(CDX);
    mMODIFYDISPLAY.insert(CDX);
    mCANSTOREADDRESS.insert(CDX);

    mSETONEZEROTOGGLE.insert(CBP);
    mINCREMENTDECREMET.insert(CBP);
    mCANSTOREADDRESS.insert(CBP);
    mGPR.insert(CBP);
    mUINTDISPLAY.insert(CBP);
    mLABELDISPLAY.insert(CBP);
    mMODIFYDISPLAY.insert(CBP);

    mSETONEZEROTOGGLE.insert(CSP);
    mINCREMENTDECREMET.insert(CSP);
    mCANSTOREADDRESS.insert(CSP);
    mGPR.insert(CSP);
    mUINTDISPLAY.insert(CSP);
    mLABELDISPLAY.insert(CSP);
    mMODIFYDISPLAY.insert(CSP);

    mSETONEZEROTOGGLE.insert(CSI);
    mINCREMENTDECREMET.insert(CSI);
    mCANSTOREADDRESS.insert(CSI);
    mGPR.insert(CSI);
    mUINTDISPLAY.insert(CSI);
    mLABELDISPLAY.insert(CSI);
    mMODIFYDISPLAY.insert(CSI);

    mSETONEZEROTOGGLE.insert(CDI);
    mINCREMENTDECREMET.insert(CDI);
    mCANSTOREADDRESS.insert(CDI);
    mGPR.insert(CDI);
    mUINTDISPLAY.insert(CDI);
    mLABELDISPLAY.insert(CDI);
    mMODIFYDISPLAY.insert(CDI);
#ifdef _WIN64
    mSETONEZEROTOGGLE.insert(R8);
    mINCREMENTDECREMET.insert(R8);
    mCANSTOREADDRESS.insert(R8);
    mGPR.insert(R8);
    mLABELDISPLAY.insert(R8);
    mUINTDISPLAY.insert(R8);
    mMODIFYDISPLAY.insert(R8);

    mSETONEZEROTOGGLE.insert(R9);
    mINCREMENTDECREMET.insert(R9);
    mCANSTOREADDRESS.insert(R9);
    mGPR.insert(R9);
    mLABELDISPLAY.insert(R9);
    mUINTDISPLAY.insert(R9);
    mMODIFYDISPLAY.insert(R9);

    mSETONEZEROTOGGLE.insert(R10);
    mINCREMENTDECREMET.insert(R10);
    mCANSTOREADDRESS.insert(R10);
    mGPR.insert(R10);
    mMODIFYDISPLAY.insert(R10);
    mUINTDISPLAY.insert(R10);
    mLABELDISPLAY.insert(R10);

    mSETONEZEROTOGGLE.insert(R11);
    mINCREMENTDECREMET.insert(R11);
    mCANSTOREADDRESS.insert(R11);
    mGPR.insert(R11);
    mMODIFYDISPLAY.insert(R11);
    mUINTDISPLAY.insert(R11);
    mLABELDISPLAY.insert(R11);

    mSETONEZEROTOGGLE.insert(R12);
    mINCREMENTDECREMET.insert(R12);
    mCANSTOREADDRESS.insert(R12);
    mGPR.insert(R12);
    mMODIFYDISPLAY.insert(R12);
    mUINTDISPLAY.insert(R12);
    mLABELDISPLAY.insert(R12);

    mSETONEZEROTOGGLE.insert(R13);
    mINCREMENTDECREMET.insert(R13);
    mCANSTOREADDRESS.insert(R13);
    mGPR.insert(R13);
    mMODIFYDISPLAY.insert(R13);
    mUINTDISPLAY.insert(R13);
    mLABELDISPLAY.insert(R13);

    mSETONEZEROTOGGLE.insert(R14);
    mINCREMENTDECREMET.insert(R14);
    mCANSTOREADDRESS.insert(R14);
    mGPR.insert(R14);
    mMODIFYDISPLAY.insert(R14);
    mUINTDISPLAY.insert(R14);
    mLABELDISPLAY.insert(R14);

    mSETONEZEROTOGGLE.insert(R15);
    mINCREMENTDECREMET.insert(R15);
    mCANSTOREADDRESS.insert(R15);
    mGPR.insert(R15);
    mMODIFYDISPLAY.insert(R15);
    mUINTDISPLAY.insert(R15);
    mLABELDISPLAY.insert(R15);
#endif //_WIN64

    mSETONEZEROTOGGLE.insert(EFLAGS);
    mGPR.insert(EFLAGS);
    mMODIFYDISPLAY.insert(EFLAGS);
    mUINTDISPLAY.insert(EFLAGS);

    // flags (we allow the user to toggle them)
    mFlags.insert(CF);
    mBOOLDISPLAY.insert(CF);
    mSETONEZEROTOGGLE.insert(CF);

    mSETONEZEROTOGGLE.insert(PF);
    mFlags.insert(PF);
    mBOOLDISPLAY.insert(PF);

    mSETONEZEROTOGGLE.insert(AF);
    mFlags.insert(AF);
    mBOOLDISPLAY.insert(AF);

    mSETONEZEROTOGGLE.insert(ZF);
    mFlags.insert(ZF);
    mBOOLDISPLAY.insert(ZF);

    mSETONEZEROTOGGLE.insert(SF);
    mFlags.insert(SF);
    mBOOLDISPLAY.insert(SF);

    mSETONEZEROTOGGLE.insert(TF);
    mFlags.insert(TF);
    mBOOLDISPLAY.insert(TF);

    mFlags.insert(IF);
    mBOOLDISPLAY.insert(IF);

    mSETONEZEROTOGGLE.insert(DF);
    mFlags.insert(DF);
    mBOOLDISPLAY.insert(DF);

    mSETONEZEROTOGGLE.insert(OF);
    mFlags.insert(OF);
    mBOOLDISPLAY.insert(OF);

    // FPU: XMM, x87 and MMX registers
    mSETONEZEROTOGGLE.insert(MxCsr);
    mDWORDDISPLAY.insert(MxCsr);
    mMODIFYDISPLAY.insert(MxCsr);
    mFPU.insert(MxCsr);

    mMODIFYDISPLAY.insert(x87r0);
    mFPUx87.insert(x87r0);
    mFPUx87_80BITSDISPLAY.insert(x87r0);
    mFPU.insert(x87r0);

    mMODIFYDISPLAY.insert(x87r1);
    mFPUx87.insert(x87r1);
    mFPUx87_80BITSDISPLAY.insert(x87r1);
    mFPU.insert(x87r1);

    mMODIFYDISPLAY.insert(x87r2);
    mFPUx87.insert(x87r2);
    mFPUx87_80BITSDISPLAY.insert(x87r2);
    mFPU.insert(x87r2);

    mMODIFYDISPLAY.insert(x87r3);
    mFPUx87.insert(x87r3);
    mFPUx87_80BITSDISPLAY.insert(x87r3);
    mFPU.insert(x87r3);

    mMODIFYDISPLAY.insert(x87r4);
    mFPUx87.insert(x87r4);
    mFPUx87_80BITSDISPLAY.insert(x87r4);
    mFPU.insert(x87r4);

    mMODIFYDISPLAY.insert(x87r5);
    mFPUx87.insert(x87r5);
    mFPU.insert(x87r5);
    mFPUx87_80BITSDISPLAY.insert(x87r5);

    mMODIFYDISPLAY.insert(x87r6);
    mFPUx87.insert(x87r6);
    mFPU.insert(x87r6);
    mFPUx87_80BITSDISPLAY.insert(x87r6);

    mMODIFYDISPLAY.insert(x87r7);
    mFPUx87.insert(x87r7);
    mFPU.insert(x87r7);
    mFPUx87_80BITSDISPLAY.insert(x87r7);

    mSETONEZEROTOGGLE.insert(x87TagWord);
    mFPUx87.insert(x87TagWord);
    mMODIFYDISPLAY.insert(x87TagWord);
    mUSHORTDISPLAY.insert(x87TagWord);
    mFPU.insert(x87TagWord);

    mSETONEZEROTOGGLE.insert(x87StatusWord);
    mUSHORTDISPLAY.insert(x87StatusWord);
    mMODIFYDISPLAY.insert(x87StatusWord);
    mFPUx87.insert(x87StatusWord);
    mFPU.insert(x87StatusWord);

    mSETONEZEROTOGGLE.insert(x87ControlWord);
    mFPUx87.insert(x87ControlWord);
    mMODIFYDISPLAY.insert(x87ControlWord);
    mUSHORTDISPLAY.insert(x87ControlWord);
    mFPU.insert(x87ControlWord);

    mSETONEZEROTOGGLE.insert(x87SW_B);
    mFPUx87.insert(x87SW_B);
    mBOOLDISPLAY.insert(x87SW_B);
    mFPU.insert(x87SW_B);

    mSETONEZEROTOGGLE.insert(x87SW_C3);
    mFPUx87.insert(x87SW_C3);
    mBOOLDISPLAY.insert(x87SW_C3);
    mFPU.insert(x87SW_C3);

    mFPUx87.insert(x87SW_TOP);
    mFIELDVALUE.insert(x87SW_TOP);
    mFPU.insert(x87SW_TOP);
    mMODIFYDISPLAY.insert(x87SW_TOP);

    mFPUx87.insert(x87SW_C2);
    mBOOLDISPLAY.insert(x87SW_C2);
    mSETONEZEROTOGGLE.insert(x87SW_C2);
    mFPU.insert(x87SW_C2);

    mSETONEZEROTOGGLE.insert(x87SW_C1);
    mFPUx87.insert(x87SW_C1);
    mBOOLDISPLAY.insert(x87SW_C1);
    mFPU.insert(x87SW_C1);

    mSETONEZEROTOGGLE.insert(x87SW_C0);
    mFPUx87.insert(x87SW_C0);
    mBOOLDISPLAY.insert(x87SW_C0);
    mFPU.insert(x87SW_C0);

    mSETONEZEROTOGGLE.insert(x87SW_IR);
    mFPUx87.insert(x87SW_IR);
    mBOOLDISPLAY.insert(x87SW_IR);
    mFPU.insert(x87SW_IR);

    mSETONEZEROTOGGLE.insert(x87SW_SF);
    mFPUx87.insert(x87SW_SF);
    mBOOLDISPLAY.insert(x87SW_SF);
    mFPU.insert(x87SW_SF);

    mSETONEZEROTOGGLE.insert(x87SW_P);
    mFPUx87.insert(x87SW_P);
    mBOOLDISPLAY.insert(x87SW_P);
    mFPU.insert(x87SW_P);

    mSETONEZEROTOGGLE.insert(x87SW_U);
    mFPUx87.insert(x87SW_U);
    mBOOLDISPLAY.insert(x87SW_U);
    mFPU.insert(x87SW_U);

    mSETONEZEROTOGGLE.insert(x87SW_O);
    mFPUx87.insert(x87SW_O);
    mBOOLDISPLAY.insert(x87SW_O);
    mFPU.insert(x87SW_O);

    mSETONEZEROTOGGLE.insert(x87SW_Z);
    mFPUx87.insert(x87SW_Z);
    mBOOLDISPLAY.insert(x87SW_Z);
    mFPU.insert(x87SW_Z);

    mSETONEZEROTOGGLE.insert(x87SW_D);
    mFPUx87.insert(x87SW_D);
    mBOOLDISPLAY.insert(x87SW_D);
    mFPU.insert(x87SW_D);

    mSETONEZEROTOGGLE.insert(x87SW_I);
    mFPUx87.insert(x87SW_I);
    mBOOLDISPLAY.insert(x87SW_I);
    mFPU.insert(x87SW_I);

    mSETONEZEROTOGGLE.insert(x87CW_IC);
    mFPUx87.insert(x87CW_IC);
    mBOOLDISPLAY.insert(x87CW_IC);
    mFPU.insert(x87CW_IC);

    mFPUx87.insert(x87CW_RC);
    mFIELDVALUE.insert(x87CW_RC);
    mFPU.insert(x87CW_RC);
    mMODIFYDISPLAY.insert(x87CW_RC);

    mFPUx87.insert(x87TW_0);
    mFIELDVALUE.insert(x87TW_0);
    mTAGWORD.insert(x87TW_0);
    mFPU.insert(x87TW_0);
    mMODIFYDISPLAY.insert(x87TW_0);

    mFPUx87.insert(x87TW_1);
    mFIELDVALUE.insert(x87TW_1);
    mTAGWORD.insert(x87TW_1);
    mFPU.insert(x87TW_1);
    mMODIFYDISPLAY.insert(x87TW_1);

    mFPUx87.insert(x87TW_2);
    mFIELDVALUE.insert(x87TW_2);
    mTAGWORD.insert(x87TW_2);
    mFPU.insert(x87TW_2);
    mMODIFYDISPLAY.insert(x87TW_2);

    mFPUx87.insert(x87TW_3);
    mFIELDVALUE.insert(x87TW_3);
    mTAGWORD.insert(x87TW_3);
    mFPU.insert(x87TW_3);
    mMODIFYDISPLAY.insert(x87TW_3);

    mFPUx87.insert(x87TW_4);
    mFIELDVALUE.insert(x87TW_4);
    mTAGWORD.insert(x87TW_4);
    mFPU.insert(x87TW_4);
    mMODIFYDISPLAY.insert(x87TW_4);

    mFPUx87.insert(x87TW_5);
    mFIELDVALUE.insert(x87TW_5);
    mTAGWORD.insert(x87TW_5);
    mFPU.insert(x87TW_5);
    mMODIFYDISPLAY.insert(x87TW_5);

    mFPUx87.insert(x87TW_6);
    mFIELDVALUE.insert(x87TW_6);
    mTAGWORD.insert(x87TW_6);
    mFPU.insert(x87TW_6);
    mMODIFYDISPLAY.insert(x87TW_6);

    mFPUx87.insert(x87TW_7);
    mFIELDVALUE.insert(x87TW_7);
    mTAGWORD.insert(x87TW_7);
    mFPU.insert(x87TW_7);
    mMODIFYDISPLAY.insert(x87TW_7);

    mFPUx87.insert(x87CW_PC);
    mFIELDVALUE.insert(x87CW_PC);
    mFPU.insert(x87CW_PC);
    mMODIFYDISPLAY.insert(x87CW_PC);

    mSETONEZEROTOGGLE.insert(x87CW_IEM);
    mFPUx87.insert(x87CW_IEM);
    mBOOLDISPLAY.insert(x87CW_IEM);
    mFPU.insert(x87CW_IEM);

    mSETONEZEROTOGGLE.insert(x87CW_PM);
    mFPUx87.insert(x87CW_PM);
    mBOOLDISPLAY.insert(x87CW_PM);
    mFPU.insert(x87CW_PM);

    mSETONEZEROTOGGLE.insert(x87CW_UM);
    mFPUx87.insert(x87CW_UM);
    mBOOLDISPLAY.insert(x87CW_UM);
    mFPU.insert(x87CW_UM);

    mSETONEZEROTOGGLE.insert(x87CW_OM);
    mFPUx87.insert(x87CW_OM);
    mBOOLDISPLAY.insert(x87CW_OM);
    mFPU.insert(x87CW_OM);

    mSETONEZEROTOGGLE.insert(x87CW_ZM);
    mFPUx87.insert(x87CW_ZM);
    mBOOLDISPLAY.insert(x87CW_ZM);
    mFPU.insert(x87CW_ZM);

    mSETONEZEROTOGGLE.insert(x87CW_DM);
    mFPUx87.insert(x87CW_DM);
    mBOOLDISPLAY.insert(x87CW_DM);
    mFPU.insert(x87CW_DM);

    mSETONEZEROTOGGLE.insert(x87CW_IM);
    mFPUx87.insert(x87CW_IM);
    mBOOLDISPLAY.insert(x87CW_IM);
    mFPU.insert(x87CW_IM);

    mSETONEZEROTOGGLE.insert(MxCsr_FZ);
    mBOOLDISPLAY.insert(MxCsr_FZ);
    mFPU.insert(MxCsr_FZ);

    mSETONEZEROTOGGLE.insert(MxCsr_PM);
    mBOOLDISPLAY.insert(MxCsr_PM);
    mFPU.insert(MxCsr_PM);

    mSETONEZEROTOGGLE.insert(MxCsr_UM);
    mBOOLDISPLAY.insert(MxCsr_UM);
    mFPU.insert(MxCsr_UM);

    mSETONEZEROTOGGLE.insert(MxCsr_OM);
    mBOOLDISPLAY.insert(MxCsr_OM);
    mFPU.insert(MxCsr_OM);

    mSETONEZEROTOGGLE.insert(MxCsr_ZM);
    mBOOLDISPLAY.insert(MxCsr_ZM);
    mFPU.insert(MxCsr_ZM);

    mSETONEZEROTOGGLE.insert(MxCsr_IM);
    mBOOLDISPLAY.insert(MxCsr_IM);
    mFPU.insert(MxCsr_IM);

    mSETONEZEROTOGGLE.insert(MxCsr_DM);
    mBOOLDISPLAY.insert(MxCsr_DM);
    mFPU.insert(MxCsr_DM);

    mSETONEZEROTOGGLE.insert(MxCsr_DAZ);
    mBOOLDISPLAY.insert(MxCsr_DAZ);
    mFPU.insert(MxCsr_DAZ);

    mSETONEZEROTOGGLE.insert(MxCsr_PE);
    mBOOLDISPLAY.insert(MxCsr_PE);
    mFPU.insert(MxCsr_PE);

    mSETONEZEROTOGGLE.insert(MxCsr_UE);
    mBOOLDISPLAY.insert(MxCsr_UE);
    mFPU.insert(MxCsr_UE);

    mSETONEZEROTOGGLE.insert(MxCsr_OE);
    mBOOLDISPLAY.insert(MxCsr_OE);
    mFPU.insert(MxCsr_OE);

    mSETONEZEROTOGGLE.insert(MxCsr_ZE);
    mBOOLDISPLAY.insert(MxCsr_ZE);
    mFPU.insert(MxCsr_ZE);

    mSETONEZEROTOGGLE.insert(MxCsr_DE);
    mBOOLDISPLAY.insert(MxCsr_DE);
    mFPU.insert(MxCsr_DE);

    mSETONEZEROTOGGLE.insert(MxCsr_IE);
    mBOOLDISPLAY.insert(MxCsr_IE);
    mFPU.insert(MxCsr_IE);

    mFIELDVALUE.insert(MxCsr_RC);
    mFPU.insert(MxCsr_RC);
    mMODIFYDISPLAY.insert(MxCsr_RC);

    mMODIFYDISPLAY.insert(MM0);
    mFPUMMX.insert(MM0);
    mFPU.insert(MM0);
    mMODIFYDISPLAY.insert(MM1);
    mFPUMMX.insert(MM1);
    mFPU.insert(MM1);
    mFPUMMX.insert(MM2);
    mMODIFYDISPLAY.insert(MM2);
    mFPU.insert(MM2);
    mFPUMMX.insert(MM3);
    mMODIFYDISPLAY.insert(MM3);
    mFPU.insert(MM3);
    mFPUMMX.insert(MM4);
    mMODIFYDISPLAY.insert(MM4);
    mFPU.insert(MM4);
    mFPUMMX.insert(MM5);
    mMODIFYDISPLAY.insert(MM5);
    mFPU.insert(MM5);
    mFPUMMX.insert(MM6);
    mMODIFYDISPLAY.insert(MM6);
    mFPU.insert(MM6);
    mFPUMMX.insert(MM7);
    mMODIFYDISPLAY.insert(MM7);
    mFPU.insert(MM7);

    mFPUXMM.insert(XMM0);
    mMODIFYDISPLAY.insert(XMM0);
    mFPU.insert(XMM0);
    mFPUXMM.insert(XMM1);
    mMODIFYDISPLAY.insert(XMM1);
    mFPU.insert(XMM1);
    mFPUXMM.insert(XMM2);
    mMODIFYDISPLAY.insert(XMM2);
    mFPU.insert(XMM2);
    mFPUXMM.insert(XMM3);
    mMODIFYDISPLAY.insert(XMM3);
    mFPU.insert(XMM3);
    mFPUXMM.insert(XMM4);
    mMODIFYDISPLAY.insert(XMM4);
    mFPU.insert(XMM4);
    mFPUXMM.insert(XMM5);
    mMODIFYDISPLAY.insert(XMM5);
    mFPU.insert(XMM5);
    mFPUXMM.insert(XMM6);
    mMODIFYDISPLAY.insert(XMM6);
    mFPU.insert(XMM6);
    mFPUXMM.insert(XMM7);
    mMODIFYDISPLAY.insert(XMM7);
    mFPU.insert(XMM7);
#ifdef _WIN64
    mFPUXMM.insert(XMM8);
    mMODIFYDISPLAY.insert(XMM8);
    mFPU.insert(XMM8);
    mFPUXMM.insert(XMM9);
    mMODIFYDISPLAY.insert(XMM9);
    mFPU.insert(XMM9);
    mFPUXMM.insert(XMM10);
    mMODIFYDISPLAY.insert(XMM10);
    mFPU.insert(XMM10);
    mFPUXMM.insert(XMM11);
    mMODIFYDISPLAY.insert(XMM11);
    mFPU.insert(XMM11);
    mFPUXMM.insert(XMM12);
    mMODIFYDISPLAY.insert(XMM12);
    mFPU.insert(XMM12);
    mFPUXMM.insert(XMM13);
    mMODIFYDISPLAY.insert(XMM13);
    mFPU.insert(XMM13);
    mFPUXMM.insert(XMM14);
    mMODIFYDISPLAY.insert(XMM14);
    mFPU.insert(XMM14);
    mFPUXMM.insert(XMM15);
    mMODIFYDISPLAY.insert(XMM15);
    mFPU.insert(XMM15);
#endif

    mFPUYMM.insert(YMM0);
    mMODIFYDISPLAY.insert(YMM0);
    mFPU.insert(YMM0);
    mFPUYMM.insert(YMM1);
    mMODIFYDISPLAY.insert(YMM1);
    mFPU.insert(YMM1);
    mFPUYMM.insert(YMM2);
    mMODIFYDISPLAY.insert(YMM2);
    mFPU.insert(YMM2);
    mFPUYMM.insert(YMM3);
    mMODIFYDISPLAY.insert(YMM3);
    mFPU.insert(YMM3);
    mFPUYMM.insert(YMM4);
    mMODIFYDISPLAY.insert(YMM4);
    mFPU.insert(YMM4);
    mFPUYMM.insert(YMM5);
    mMODIFYDISPLAY.insert(YMM5);
    mFPU.insert(YMM5);
    mFPUYMM.insert(YMM6);
    mMODIFYDISPLAY.insert(YMM6);
    mFPU.insert(YMM6);
    mFPUYMM.insert(YMM7);
    mMODIFYDISPLAY.insert(YMM7);
    mFPU.insert(YMM7);

#ifdef _WIN64
    mFPUYMM.insert(YMM8);
    mMODIFYDISPLAY.insert(YMM8);
    mFPU.insert(YMM8);
    mFPUYMM.insert(YMM9);
    mMODIFYDISPLAY.insert(YMM9);
    mFPU.insert(YMM9);
    mFPUYMM.insert(YMM10);
    mMODIFYDISPLAY.insert(YMM10);
    mFPU.insert(YMM10);
    mFPUYMM.insert(YMM11);
    mMODIFYDISPLAY.insert(YMM11);
    mFPU.insert(YMM11);
    mFPUYMM.insert(YMM12);
    mMODIFYDISPLAY.insert(YMM12);
    mFPU.insert(YMM12);
    mFPUYMM.insert(YMM13);
    mMODIFYDISPLAY.insert(YMM13);
    mFPU.insert(YMM13);
    mFPUYMM.insert(YMM14);
    mMODIFYDISPLAY.insert(YMM14);
    mFPU.insert(YMM14);
    mFPUYMM.insert(YMM15);
    mMODIFYDISPLAY.insert(YMM15);
    mFPU.insert(YMM15);
#endif
    //registers that should not be changed
    mNoChange.insert(LastError);

    mNoChange.insert(GS);
    mUSHORTDISPLAY.insert(GS);
    mSEGMENTREGISTER.insert(GS);

    mNoChange.insert(FS);
    mUSHORTDISPLAY.insert(FS);
    mSEGMENTREGISTER.insert(FS);

    mNoChange.insert(ES);
    mUSHORTDISPLAY.insert(ES);
    mSEGMENTREGISTER.insert(ES);

    mNoChange.insert(DS);
    mUSHORTDISPLAY.insert(DS);
    mSEGMENTREGISTER.insert(DS);

    mNoChange.insert(CS);
    mUSHORTDISPLAY.insert(CS);
    mSEGMENTREGISTER.insert(CS);

    mNoChange.insert(SS);
    mUSHORTDISPLAY.insert(SS);
    mSEGMENTREGISTER.insert(SS);

    mNoChange.insert(DR0);
    mUINTDISPLAY.insert(DR0);
    mLABELDISPLAY.insert(DR0);
    mONLYMODULEANDLABELDISPLAY.insert(DR0);
    mCANSTOREADDRESS.insert(DR0);

    mNoChange.insert(DR1);
    mONLYMODULEANDLABELDISPLAY.insert(DR1);
    mUINTDISPLAY.insert(DR1);
    mCANSTOREADDRESS.insert(DR1);

    mLABELDISPLAY.insert(DR2);
    mONLYMODULEANDLABELDISPLAY.insert(DR2);
    mNoChange.insert(DR2);
    mUINTDISPLAY.insert(DR2);
    mCANSTOREADDRESS.insert(DR2);

    mNoChange.insert(DR3);
    mONLYMODULEANDLABELDISPLAY.insert(DR3);
    mLABELDISPLAY.insert(DR3);
    mUINTDISPLAY.insert(DR3);
    mCANSTOREADDRESS.insert(DR3);

    mNoChange.insert(DR6);
    mLABELDISPLAY.insert(DR6);
    mONLYMODULEANDLABELDISPLAY.insert(DR6);
    mUINTDISPLAY.insert(DR6);
    mCANSTOREADDRESS.insert(DR6);

    mNoChange.insert(DR7);
    mUINTDISPLAY.insert(DR7);
    mONLYMODULEANDLABELDISPLAY.insert(DR7);
    mCANSTOREADDRESS.insert(DR7);
    mLABELDISPLAY.insert(DR7);

    mNoChange.insert(CIP);
    mUINTDISPLAY.insert(CIP);
    mLABELDISPLAY.insert(CIP);
    mONLYMODULEANDLABELDISPLAY.insert(CIP);
    mCANSTOREADDRESS.insert(CIP);

    InitMappings();

    fontsUpdatedSlot();
    connect(Config(), SIGNAL(fontsUpdated()), this, SLOT(fontsUpdatedSlot()));

    memset(&wRegDumpStruct, 0, sizeof(REGDUMP));
    memset(&wCipRegDumpStruct, 0, sizeof(REGDUMP));
    mCip = 0;
    mRegisterUpdates.clear();

    mButtonHeight = 0;
    yTopSpacing = 4; //set top spacing (in pixels)

    // Context Menu
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    // foreign messages
    connect(Bridge::getBridge(), SIGNAL(updateRegisters()), this, SLOT(updateRegistersSlot()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayCustomContextMenuSlot(QPoint)));
    connect(Bridge::getBridge(), SIGNAL(dbgStateChanged(DBGSTATE)), this, SLOT(debugStateChangedSlot(DBGSTATE)));
    // self communication for repainting (maybe some other widgets needs this information, too)
    connect(this, SIGNAL(refresh()), this, SLOT(repaint()));
    // context menu actions
    connect(wCM_Increment, SIGNAL(triggered()), this, SLOT(onIncrementAction()));
    connect(wCM_ChangeFPUView, SIGNAL(triggered()), this, SLOT(onChangeFPUViewAction()));
    connect(wCM_Decrement, SIGNAL(triggered()), this, SLOT(onDecrementAction()));
    connect(wCM_Incrementx87Stack, SIGNAL(triggered()), this, SLOT(onIncrementx87StackAction()));
    connect(wCM_Decrementx87Stack, SIGNAL(triggered()), this, SLOT(onDecrementx87StackAction()));
    connect(wCM_Zero, SIGNAL(triggered()), this, SLOT(onZeroAction()));
    connect(wCM_SetToOne, SIGNAL(triggered()), this, SLOT(onSetToOneAction()));
    connect(wCM_Modify, SIGNAL(triggered()), this, SLOT(onModifyAction()));
    connect(wCM_ToggleValue, SIGNAL(triggered()), this, SLOT(onToggleValueAction()));
    connect(wCM_CopyToClipboard, SIGNAL(triggered()), this, SLOT(onCopyToClipboardAction()));
    connect(wCM_CopySymbolToClipboard, SIGNAL(triggered()), this, SLOT(onCopySymbolToClipboardAction()));
    connect(wCM_CopyAll, SIGNAL(triggered()), this, SLOT(onCopyAllAction()));
    connect(wCM_FollowInDisassembly, SIGNAL(triggered()), this, SLOT(onFollowInDisassembly()));
    connect(wCM_FollowInDump, SIGNAL(triggered()), this, SLOT(onFollowInDump()));
    connect(wCM_FollowInStack, SIGNAL(triggered()), this, SLOT(onFollowInStack()));
    connect(wCM_IncrementPtrSize, SIGNAL(triggered()), this, SLOT(onIncrementPtrSize()));
    connect(wCM_DecrementPtrSize, SIGNAL(triggered()), this, SLOT(onDecrementPtrSize()));
    connect(wCM_Push, SIGNAL(triggered()), this, SLOT(onPushAction()));
    connect(wCM_Pop, SIGNAL(triggered()), this, SLOT(onPopAction()));

    refreshShortcutsSlot();
    connect(Config(), SIGNAL(shortcutsUpdated()), this, SLOT(refreshShortcutsSlot()));
}

void RegistersView::refreshShortcutsSlot()
{
    wCM_Increment->setShortcut(ConfigShortcut("ActionIncreaseRegister"));
    wCM_Decrement->setShortcut(ConfigShortcut("ActionDecreaseRegister"));
    wCM_Zero->setShortcut(ConfigShortcut("ActionZeroRegister"));
    wCM_SetToOne->setShortcut(ConfigShortcut("ActionSetOneRegister"));
    wCM_ToggleValue->setShortcut(ConfigShortcut("ActionToggleRegisterValue"));
    wCM_CopyToClipboard->setShortcut(ConfigShortcut("ActionCopy"));
    wCM_CopySymbolToClipboard->setShortcut(ConfigShortcut("ActionCopySymbol"));
    wCM_CopyAll->setShortcut(ConfigShortcut("ActionCopyAllRegisters"));
}

RegistersView::~RegistersView()
{
}

void RegistersView::fontsUpdatedSlot()
{
    auto font = ConfigFont("Registers");
    setFont(font);
    if(mChangeViewButton)
        mChangeViewButton->setFont(font);
    int wRowsHeight = QFontMetrics(this->font()).height();
    wRowsHeight = (wRowsHeight * 105) / 100;
    wRowsHeight = (wRowsHeight % 2) == 0 ? wRowsHeight : wRowsHeight + 1;
    mRowHeight = wRowsHeight;
    mCharWidth = QFontMetrics(this->font()).averageCharWidth();
    setFixedHeight(getEstimateHeight());
    repaint();
}

void RegistersView::ShowFPU(bool set_showfpu)
{
    mShowFpu = set_showfpu;
    InitMappings();
    repaint();
}


/**
 * @brief retrieves the register id from given corrdinates of the viewport
 * @param line
 * @param offset (x-coord)
 * @param resulting register-id
 * @return true if register found
 */
bool RegistersView::identifyRegister(const int line, const int offset, REGISTER_NAME* clickedReg)
{
    // we start by an unknown register id
    if(clickedReg)
        *clickedReg = UNKNOWN;
    bool found_flag = false;
    QMap<REGISTER_NAME, Register_Position>::const_iterator it = mRegisterPlaces.begin();
    // iterate all registers that being displayed
    while(it != mRegisterPlaces.end())
    {
        if((it.value().line == (line - mVScrollOffset))    /* same line ? */
                && ((1 + it.value().start) <= offset)   /* between start ... ? */
                && (offset <= (1 + it.value().start + it.value().labelwidth + it.value().valuesize)) /* ... and end ? */
          )
        {
            // we found a matching register in the viewport
            if(clickedReg)
                *clickedReg = (REGISTER_NAME)it.key();
            found_flag = true;
            break;

        }
        ++it;
    }
    return found_flag;
}

void RegistersView::mousePressEvent(QMouseEvent* event)
{
    if(!DbgIsDebugging())
        return;

    if(event->y() < yTopSpacing - mButtonHeight)
    {
        onChangeFPUViewAction();
    }
    else
    {
        // get mouse position
        const int y = (event->y() - yTopSpacing) / (double)mRowHeight;
        const int x = event->x() / (double)mCharWidth;

        REGISTER_NAME r;
        // do we find a corresponding register?
        if(identifyRegister(y, x, &r))
        {
            mSelected = r;
            emit refresh();
        }
        else
            mSelected = UNKNOWN;
    }
}

void RegistersView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    if(!DbgIsDebugging() || event->button() != Qt::LeftButton)
        return;
    // get mouse position
    const int y = (event->y() - yTopSpacing) / (double)mRowHeight;
    const int x = event->x() / (double)mCharWidth;

    // do we find a corresponding register?
    if(!identifyRegister(y, x, 0))
        return;
    // is current register general purposes register or FPU register?
    if(mMODIFYDISPLAY.contains(mSelected))
    {
        wCM_Modify->trigger();
    }
    else if(mBOOLDISPLAY.contains(mSelected))  // is flag ?
        wCM_ToggleValue->trigger();
    else if(mSelected == CIP) //double clicked on CIP register
        DbgCmdExec("disasm cip");
}

void RegistersView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    if(mChangeViewButton != NULL)
    {
        if(mShowFpu)
            mChangeViewButton->setText(tr("Hide FPU"));
        else
            mChangeViewButton->setText(tr("Show FPU"));
    }

    QPainter wPainter(this->viewport());
    wPainter.fillRect(wPainter.viewport(), QBrush(ConfigColor("RegistersBackgroundColor")));

    // Don't draw the registers if a program isn't actually running
    if(!DbgIsDebugging())
        return;

    // Iterate all registers
    for(auto itr = mRegisterMapping.begin(); itr != mRegisterMapping.end(); itr++)
    {
        // Paint register at given position
        drawRegister(&wPainter, itr.key(), registerValue(&wRegDumpStruct, itr.key()));
    }
}

void RegistersView::keyPressEvent(QKeyEvent* event)
{
    if(!DbgIsDebugging())
        return;
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        wCM_Modify->trigger();
}

QSize RegistersView::sizeHint() const
{
    // 32 character width
    return QSize(32 * mCharWidth , this->viewport()->height());
}

void* RegistersView::operator new(size_t size)
{
    return _aligned_malloc(size, 16);
}

void RegistersView::operator delete(void* p)
{
    _aligned_free(p);
}

/**
 * @brief                   Get the label associated with the register
 * @param register_selected the register
 * @return                  the label
 */
QString RegistersView::getRegisterLabel(REGISTER_NAME register_selected)
{
    char label_text[MAX_LABEL_SIZE] = "";
    char module_text[MAX_MODULE_SIZE] = "";
    char string_text[MAX_STRING_SIZE] = "";

    QString valueText = QString("%1").arg((* ((duint*) registerValue(&wRegDumpStruct, register_selected))), mRegisterPlaces[register_selected].valuesize, 16, QChar('0')).toUpper();
    duint register_value = (* ((duint*) registerValue(&wRegDumpStruct, register_selected)));
    QString newText = QString("");

    bool hasString = DbgGetStringAt(register_value, string_text);
    bool hasLabel = DbgGetLabelAt(register_value, SEG_DEFAULT, label_text);
    bool hasModule = DbgGetModuleAt(register_value, module_text);

    if(hasString && !mONLYMODULEANDLABELDISPLAY.contains(register_selected))
    {
        newText = string_text;
    }
    else if(hasLabel && hasModule)
    {
        newText = "<" + QString(module_text) + "." + QString(label_text) + ">";
    }
    else if(hasModule)
    {
        newText = QString(module_text) + "." + valueText;
    }
    else if(hasLabel)
    {
        newText = "<" + QString(label_text) + ">";
    }
    else if(!mONLYMODULEANDLABELDISPLAY.contains(register_selected))
    {
        bool isCharacter = false;
        if(register_value == (register_value & 0xFF))
        {
            QChar c = QChar((char)register_value);
            if(c.isPrint())
            {
                newText = QString("'%1'").arg((char)register_value);
                isCharacter = IsCharacterRegister(register_selected);
            }
        }
        else if(register_value == (register_value & 0xFFF))  //UNICODE?
        {
            QChar c = QChar((wchar_t)register_value);
            if(c.isPrint())
            {
                newText = "L'" + QString(c) + "'";
                isCharacter = IsCharacterRegister(register_selected);
            }
        }
    }

    return newText;
}

QString RegistersView::GetRegStringValueFromValue(REGISTER_NAME reg, char* value)
{
    QString valueText;

    if(mUINTDISPLAY.contains(reg))
        valueText = QString("%1").arg((* ((duint*) value)), mRegisterPlaces[reg].valuesize, 16, QChar('0')).toUpper();
    else if(mUSHORTDISPLAY.contains(reg))
        valueText = QString("%1").arg((* ((unsigned short*) value)), mRegisterPlaces[reg].valuesize, 16, QChar('0')).toUpper();
    else if(mDWORDDISPLAY.contains(reg))
        valueText = QString("%1").arg((* ((DWORD*) value)), mRegisterPlaces[reg].valuesize, 16, QChar('0')).toUpper();
    else if(mBOOLDISPLAY.contains(reg))
        valueText = QString("%1").arg((* ((bool*) value)), mRegisterPlaces[reg].valuesize, 16, QChar('0')).toUpper();
    else if(mFIELDVALUE.contains(reg))
    {
        if(mTAGWORD.contains(reg))
        {
            valueText = QString("%1").arg((* ((unsigned short*) value)), 1, 16, QChar('0')).toUpper();
            valueText += QString(" (");
            valueText += GetTagWordStateString((* ((unsigned short*) value)));
            valueText += QString(")");
        }
        if(reg == MxCsr_RC)
        {
            valueText = QString("%1").arg((* ((unsigned short*) value)), 1, 16, QChar('0')).toUpper();
            valueText += QString(" (");
            valueText += GetMxCsrRCStateString((* ((unsigned short*) value)));
            valueText += QString(")");
        }
        else if(reg == x87CW_RC)
        {
            valueText = QString("%1").arg((* ((unsigned short*) value)), 1, 16, QChar('0')).toUpper();
            valueText += QString(" (");
            valueText += GetControlWordRCStateString((* ((unsigned short*) value)));
            valueText += QString(")");
        }
        else if(reg == x87CW_PC)
        {
            valueText = QString("%1").arg((* ((unsigned short*) value)), 1, 16, QChar('0')).toUpper();
            valueText += QString(" (");
            valueText += GetControlWordPCStateString((* ((unsigned short*) value)));
            valueText += QString(")");
        }
        else if(reg == x87SW_TOP)
        {
            valueText = QString("%1").arg((* ((unsigned short*) value)), 1, 16, QChar('0')).toUpper();
            valueText += QString(" (ST0=");
            valueText += GetStatusWordTOPStateString((* ((unsigned short*) value)));
            valueText += QString(")");
        }
    }
    else if(reg == LastError)
    {
        LASTERROR* data = (LASTERROR*)value;
        if(*data->name)
            valueText = QString().sprintf("%08X (%s)", data->code, data->name);
        else
            valueText = QString().sprintf("%08X", data->code);
        mRegisterPlaces[LastError].valuesize = valueText.length();
    }
    else
    {
        SIZE_T size = GetSizeRegister(reg);
        if(size != 0)
        {
            if(ConfigBool("Gui", "FpuRegistersLittleEndian"))
                valueText = QString(QByteArray(value, size).toHex()).toUpper();
            else
                valueText = QString(ByteReverse(QByteArray(value, size)).toHex()).toUpper();
        }
        else
            valueText = QString("???");
    }

    return valueText;
}

#define MxCsr_RC_NEAR 0
#define MxCsr_RC_NEGATIVE 1
#define MxCsr_RC_POSITIVE   2
#define MxCsr_RC_TOZERO 3

STRING_VALUE_TABLE_t MxCsrRCValueStringTable[] =
{
    {"Toward Zero", MxCsr_RC_TOZERO},
    {"Toward Positive", MxCsr_RC_POSITIVE},
    {"Toward Negative", MxCsr_RC_NEGATIVE},
    {"Round Near", MxCsr_RC_NEAR}
};

unsigned int RegistersView::GetMxCsrRCValueFromString(QString string)
{
    int i;

    for(i = 0; i < (sizeof(MxCsrRCValueStringTable) / sizeof(*MxCsrRCValueStringTable)); i++)
    {
        if(MxCsrRCValueStringTable[i].string == string)
            return MxCsrRCValueStringTable[i].value;
    }

    return i;
}

QString RegistersView::GetMxCsrRCStateString(unsigned short state)
{
    int i;

    for(i = 0; i < (sizeof(MxCsrRCValueStringTable) / sizeof(*MxCsrRCValueStringTable)); i++)
    {
        if(MxCsrRCValueStringTable[i].value == state)
            return MxCsrRCValueStringTable[i].string;
    }

    return "Unknown";
}

#define x87CW_RC_NEAR 0
#define x87CW_RC_DOWN 1
#define x87CW_RC_UP   2
#define x87CW_RC_TRUNCATE 3

STRING_VALUE_TABLE_t ControlWordRCValueStringTable[] =
{
    {"Truncate", x87CW_RC_TRUNCATE},
    {"Round Up", x87CW_RC_UP},
    {"Round Down", x87CW_RC_DOWN},
    {"Round Near", x87CW_RC_NEAR}
};

unsigned int RegistersView::GetControlWordRCValueFromString(QString string)
{
    int i;

    for(i = 0; i < (sizeof(ControlWordRCValueStringTable) / sizeof(*ControlWordRCValueStringTable)); i++)
    {
        if(ControlWordRCValueStringTable[i].string == string)
            return ControlWordRCValueStringTable[i].value;
    }

    return i;
}

QString RegistersView::GetControlWordRCStateString(unsigned short state)
{
    int i;

    for(i = 0; i < (sizeof(ControlWordRCValueStringTable) / sizeof(*ControlWordRCValueStringTable)); i++)
    {
        if(ControlWordRCValueStringTable[i].value == state)
            return ControlWordRCValueStringTable[i].string;
    }

    return "Unknown";
}

#define x87SW_TOP_0 0
#define x87SW_TOP_1 1
#define x87SW_TOP_2 2
#define x87SW_TOP_3 3
#define x87SW_TOP_4 4
#define x87SW_TOP_5 5
#define x87SW_TOP_6 6
#define x87SW_TOP_7 7

STRING_VALUE_TABLE_t StatusWordTOPValueStringTable[] =
{
    {"x87r0", x87SW_TOP_0},
    {"x87r1", x87SW_TOP_1},
    {"x87r2", x87SW_TOP_2},
    {"x87r3", x87SW_TOP_3},
    {"x87r4", x87SW_TOP_4},
    {"x87r5", x87SW_TOP_5},
    {"x87r6", x87SW_TOP_6},
    {"x87r7", x87SW_TOP_7}
};

unsigned int RegistersView::GetStatusWordTOPValueFromString(QString string)
{
    int i;

    for(i = 0; i < (sizeof(StatusWordTOPValueStringTable) / sizeof(*StatusWordTOPValueStringTable)); i++)
    {
        if(StatusWordTOPValueStringTable[i].string == string)
            return StatusWordTOPValueStringTable[i].value;
    }

    return i;
}

QString RegistersView::GetStatusWordTOPStateString(unsigned short state)
{
    int i;

    for(i = 0; i < (sizeof(StatusWordTOPValueStringTable) / sizeof(*StatusWordTOPValueStringTable)); i++)
    {
        if(StatusWordTOPValueStringTable[i].value == state)
            return StatusWordTOPValueStringTable[i].string;
    }

    return "Unknown";
}


#define x87CW_PC_REAL4 0
#define x87CW_PC_NOTUSED 1
#define x87CW_PC_REAL8   2
#define x87CW_PC_REAL10 3

STRING_VALUE_TABLE_t ControlWordPCValueStringTable[] =
{
    {"Real4", x87CW_PC_REAL4},
    {"Not Used", x87CW_PC_NOTUSED},
    {"Real8", x87CW_PC_REAL8},
    {"Real10", x87CW_PC_REAL10}
};


unsigned int RegistersView::GetControlWordPCValueFromString(QString string)
{
    int i;

    for(i = 0; i < (sizeof(ControlWordPCValueStringTable) / sizeof(*ControlWordPCValueStringTable)); i++)
    {
        if(ControlWordPCValueStringTable[i].string == string)
            return ControlWordPCValueStringTable[i].value;
    }

    return i;
}


QString RegistersView::GetControlWordPCStateString(unsigned short state)
{
    int i;

    for(i = 0; i < (sizeof(ControlWordPCValueStringTable) / sizeof(*ControlWordPCValueStringTable)); i++)
    {
        if(ControlWordPCValueStringTable[i].value == state)
            return ControlWordPCValueStringTable[i].string;
    }

    return "Unknown";
}


#define X87FPU_TAGWORD_NONZERO 0
#define X87FPU_TAGWORD_ZERO 1
#define X87FPU_TAGWORD_SPECIAL 2
#define X87FPU_TAGWORD_EMPTY 3

STRING_VALUE_TABLE_t TagWordValueStringTable[] =
{
    {"Nonzero", X87FPU_TAGWORD_NONZERO},
    {"Zero", X87FPU_TAGWORD_ZERO},
    {"Special", X87FPU_TAGWORD_SPECIAL},
    {"Empty", X87FPU_TAGWORD_EMPTY}
};

unsigned int RegistersView::GetTagWordValueFromString(QString string)
{
    int i;

    for(i = 0; i < (sizeof(TagWordValueStringTable) / sizeof(*TagWordValueStringTable)); i++)
    {
        if(TagWordValueStringTable[i].string == string)
            return TagWordValueStringTable[i].value;
    }

    return i;
}


QString RegistersView::GetTagWordStateString(unsigned short state)
{
    int i;

    for(i = 0; i < (sizeof(TagWordValueStringTable) / sizeof(*TagWordValueStringTable)); i++)
    {
        if(TagWordValueStringTable[i].value == state)
            return TagWordValueStringTable[i].string;
    }

    return "Unknown";
}

void RegistersView::drawRegister(QPainter* p, REGISTER_NAME reg, char* value)
{
    // is the register-id known?
    if(mRegisterMapping.contains(reg))
    {
        // padding to the left is at least one character (looks better)
        int x = mCharWidth * (1 + mRegisterPlaces[reg].start);
        int ySpace = yTopSpacing;
        if(mVScrollOffset != 0)
            ySpace = 0;
        int y = mRowHeight * (mRegisterPlaces[reg].line + mVScrollOffset) + ySpace;

        //draw raster
        /*
        p->save();
        p->setPen(QColor("#FF0000"));
        p->drawLine(0, y, this->viewport()->width(), y);
        p->restore();
        */

        // draw name of value
        int width = mCharWidth * mRegisterMapping[reg].length();

        // set the color of the register label
#ifdef _WIN64
        switch(reg)
        {
        case CCX: //arg1
        case CDX: //arg2
        case R8: //arg3
        case R9: //arg4
            p->setPen(ConfigColor("RegistersArgumentLabelColor"));
            break;
        default:
#endif //_WIN64
            p->setPen(ConfigColor("RegistersLabelColor"));
#ifdef _WIN64
            break;
        }
#endif //_WIN64

        p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, mRegisterMapping[reg]);
        x += (mRegisterPlaces[reg].labelwidth) * mCharWidth;
        //p->drawText(offset,mRowHeight*(mRegisterPlaces[reg].line+1),mRegisterMapping[reg]);

        //set highlighting
        if(DbgIsDebugging() && mRegisterUpdates.contains(reg))
            p->setPen(ConfigColor("RegistersModifiedColor"));
        else
            p->setPen(ConfigColor("RegistersColor"));

        //get register value
        QString valueText = GetRegStringValueFromValue(reg, value);

        //selection
        if(mSelected == reg)
        {
            p->fillRect(x, y, mRegisterPlaces[reg].valuesize * mCharWidth, mRowHeight, QBrush(ConfigColor("RegistersSelectionColor")));
            //p->fillRect(QRect(x + (mRegisterPlaces[reg].labelwidth)*mCharWidth ,mRowHeight*(mRegisterPlaces[reg].line)+2, mRegisterPlaces[reg].valuesize*mCharWidth, mRowHeight), QBrush(ConfigColor("RegistersSelectionColor")));
        }

        // draw value
        width = mCharWidth * valueText.length();
        p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, valueText);
        //p->drawText(x + (mRegisterPlaces[reg].labelwidth)*mCharWidth ,mRowHeight*(mRegisterPlaces[reg].line+1),QString("%1").arg(value, mRegisterPlaces[reg].valuesize, 16, QChar('0')).toUpper());

        x += valueText.length() * mCharWidth;

        if(mFPUx87_80BITSDISPLAY.contains(reg) && DbgIsDebugging())
        {
            p->setPen(ConfigColor("RegistersExtraInfoColor"));
            x += 1 * mCharWidth; //1 space
            QString newText;
            if(mRegisterUpdates.contains(x87SW_TOP))
                p->setPen(ConfigColor("RegistersModifiedColor"));

            newText = QString("ST%1 ").arg(((X87FPUREGISTER*) registerValue(&wRegDumpStruct, reg))->st_value);
            width = newText.length() * mCharWidth;
            p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, newText);

            x += width;

            newText = QString("");

            p->setPen(ConfigColor("RegistersExtraInfoColor"));

            if(reg == x87r0 && mRegisterUpdates.contains(x87TW_0))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r1 && mRegisterUpdates.contains(x87TW_1))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r2 && mRegisterUpdates.contains(x87TW_2))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r3 && mRegisterUpdates.contains(x87TW_3))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r4 && mRegisterUpdates.contains(x87TW_4))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r5 && mRegisterUpdates.contains(x87TW_5))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r6 && mRegisterUpdates.contains(x87TW_6))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }
            else if(reg == x87r7 && mRegisterUpdates.contains(x87TW_7))
            {
                p->setPen(ConfigColor("RegistersModifiedColor"));
            }

            newText += GetTagWordStateString(((X87FPUREGISTER*) registerValue(&wRegDumpStruct, reg))->tag) + QString(" ");

            width = newText.length() * mCharWidth;
            p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, newText);

            x += width;

            newText = QString("");

            p->setPen(ConfigColor("RegistersExtraInfoColor"));

            if(DbgIsDebugging() && mRegisterUpdates.contains(reg))
                p->setPen(ConfigColor("RegistersModifiedColor"));

            newText += ToLongDoubleString(((X87FPUREGISTER*) registerValue(&wRegDumpStruct, reg))->data);
            width = newText.length() * mCharWidth;
            p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, newText);
        }

        // do we have a label ?
        if(mLABELDISPLAY.contains(reg))
        {
            x += 5 * mCharWidth; //5 spaces

            QString newText = getRegisterLabel(reg);

            // are there additional informations?
            if(newText != "")
            {
                width = newText.length() * mCharWidth;
                p->setPen(ConfigColor("RegistersExtraInfoColor"));
                p->drawText(x, y, width, mRowHeight, Qt::AlignVCenter, newText);
                //p->drawText(x,mRowHeight*(mRegisterPlaces[reg].line+1),newText);
            }
        }
    }
}

void RegistersView::updateRegistersSlot()
{
    // read registers
    REGDUMP z;
    memset(&z, 0, sizeof(REGDUMP));
    DbgGetRegDump(&z);
    // update gui
    setRegisters(&z);
}

void RegistersView::ModifyFields(QString title, STRING_VALUE_TABLE_t* table, SIZE_T size)
{
    SelectFields mSelectFields(this);
    QListWidget* mQListWidget = mSelectFields.GetList();

    QStringList items;
    unsigned int i;

    for(i = 0; i < size; i++)
        items << table[i].string;

    mQListWidget->addItems(items);

    mSelectFields.setWindowTitle(title);
    if(mSelectFields.exec() != QDialog::Accepted)
        return;

    if(mQListWidget->selectedItems().count() != 1)
        return;

    QListWidgetItem* item = mQListWidget->takeItem(mQListWidget->currentRow());

    duint value;

    for(i = 0; i < size; i++)
    {
        if(table[i].string == item->text())
            break;
    }

    value = table[i].value;

    setRegister(mSelected, (duint)value);
}

#define MODIFY_FIELDS_DISPLAY(title, table) ModifyFields(tr("Edit") + QChar(' ') + QString(title), (STRING_VALUE_TABLE_t *) & table, SIZE_TABLE(table) )

/**
 * @brief   This function displays the appropriate edit dialog according to selected register
 * @return  Nothing.
 */

void RegistersView::displayEditDialog()
{
    if(mFPU.contains(mSelected))
    {
        if(mTAGWORD.contains(mSelected))
            MODIFY_FIELDS_DISPLAY("Tag " + mRegisterMapping.constFind(mSelected).value(), TagWordValueStringTable);
        else if(mSelected == MxCsr_RC)
            MODIFY_FIELDS_DISPLAY("MxCsr_RC", MxCsrRCValueStringTable);
        else if(mSelected == x87CW_RC)
            MODIFY_FIELDS_DISPLAY("x87CW_RC", ControlWordRCValueStringTable);
        else if(mSelected == x87CW_PC)
            MODIFY_FIELDS_DISPLAY("x87CW_PC", ControlWordPCValueStringTable);
        else if(mSelected == x87SW_TOP)
            MODIFY_FIELDS_DISPLAY("x87SW_TOP ST0=", StatusWordTOPValueStringTable);
        else if(mFPUYMM.contains(mSelected))
        {
            EditFloatRegister mEditFloat(256, this);
            mEditFloat.setWindowTitle(tr("Edit YMM register"));
            mEditFloat.loadData(registerValue(&wRegDumpStruct, mSelected));
            mEditFloat.show();
            mEditFloat.selectAllText();
            if(mEditFloat.exec() == QDialog::Accepted)
                setRegister(mSelected, (duint)mEditFloat.getData());
        }
        else if(mFPUXMM.contains(mSelected))
        {
            EditFloatRegister mEditFloat(128, this);
            mEditFloat.setWindowTitle(tr("Edit XMM register"));
            mEditFloat.loadData(registerValue(&wRegDumpStruct, mSelected));
            if(mEditFloat.exec() == QDialog::Accepted)
                setRegister(mSelected, (duint)mEditFloat.getData());
        }
        else
        {
            bool errorinput = false;
            LineEditDialog mLineEdit(this);

            mLineEdit.setText(GetRegStringValueFromValue(mSelected,  registerValue(&wRegDumpStruct, mSelected)));
            mLineEdit.setWindowTitle("Edit FPU register");
            mLineEdit.setWindowIcon(DIcon("log.png"));
            mLineEdit.setCursorPosition(0);
            mLineEdit.ForceSize(GetSizeRegister(mSelected) * 2);
            do
            {
                errorinput = false;
                mLineEdit.show();
                mLineEdit.selectAllText();
                if(mLineEdit.exec() != QDialog::Accepted)
                    return; //pressed cancel
                else
                {
                    bool ok = false;
                    duint fpuvalue;

                    if(mUSHORTDISPLAY.contains(mSelected))
                        fpuvalue = (duint) mLineEdit.editText.toUShort(&ok, 16);
                    else if(mDWORDDISPLAY.contains(mSelected))
                        fpuvalue = mLineEdit.editText.toUInt(&ok, 16);
                    else if(mFPUMMX.contains(mSelected) || mFPUXMM.contains(mSelected) || mFPUYMM.contains(mSelected) || mFPUx87_80BITSDISPLAY.contains(mSelected))
                    {
                        QByteArray pArray =  mLineEdit.editText.toLocal8Bit();
                        if(pArray.size() == GetSizeRegister(mSelected) * 2)
                        {
                            char* pData = (char*) calloc(1, sizeof(char) * GetSizeRegister(mSelected));

                            if(pData != NULL)
                            {
                                ok = true;
                                char actual_char[3];
                                unsigned int i;
                                for(i = 0; i < GetSizeRegister(mSelected); i++)
                                {
                                    memset(actual_char, 0, sizeof(actual_char));
                                    memcpy(actual_char, (char*) pArray.data() + (i * 2), 2);
                                    if(! isxdigit(actual_char[0]) || ! isxdigit(actual_char[1]))
                                    {
                                        ok = false;
                                        break;
                                    }
                                    pData[i] = (char)strtol(actual_char, NULL, 16);
                                }

                                if(ok)
                                    setRegister(mSelected, (duint) pData);

                                free(pData);

                                if(ok)
                                    return;
                            }
                        }
                    }
                    if(!ok)
                    {
                        errorinput = true;

                        SimpleWarningBox(this, tr("ERROR CONVERTING TO HEX"), tr("ERROR CONVERTING TO HEX"));
                    }
                    else
                        setRegister(mSelected, fpuvalue);
                }

            }
            while(errorinput);
        }
    }
    else
    {
        WordEditDialog wEditDial(this);
        wEditDial.setup(tr("Edit"), (* ((duint*) registerValue(&wRegDumpStruct, mSelected))), sizeof(dsint));
        if(wEditDial.exec() == QDialog::Accepted) //OK button clicked
            setRegister(mSelected, wEditDial.getVal());
    }
}

void RegistersView::onIncrementx87StackAction()
{
    if(mFPUx87_80BITSDISPLAY.contains(mSelected))
        setRegister(x87SW_TOP, ((* ((duint*) registerValue(&wRegDumpStruct, x87SW_TOP))) + 1) % 8);
}

void RegistersView::onDecrementx87StackAction()
{
    if(mFPUx87_80BITSDISPLAY.contains(mSelected))
        setRegister(x87SW_TOP, ((* ((duint*) registerValue(&wRegDumpStruct, x87SW_TOP))) - 1) % 8);
}

void RegistersView::onIncrementAction()
{
    if(mINCREMENTDECREMET.contains(mSelected))
        setRegister(mSelected, (* ((duint*) registerValue(&wRegDumpStruct, mSelected))) + 1);
}

void RegistersView::onDecrementAction()
{
    if(mINCREMENTDECREMET.contains(mSelected))
        setRegister(mSelected, (* ((duint*) registerValue(&wRegDumpStruct, mSelected))) - 1);
}

void RegistersView::onIncrementPtrSize()
{
    if(mINCREMENTDECREMET.contains(mSelected))
        setRegister(mSelected, (* ((duint*) registerValue(&wRegDumpStruct, mSelected))) + sizeof(void*));
}

void RegistersView::onDecrementPtrSize()
{
    if(mINCREMENTDECREMET.contains(mSelected))
        setRegister(mSelected, (* ((duint*) registerValue(&wRegDumpStruct, mSelected))) - sizeof(void*));
}

void RegistersView::onPushAction()
{
    duint csp = (* ((duint*) registerValue(&wRegDumpStruct, CSP))) - sizeof(void*);
    duint regVal = 0;
    if(mSEGMENTREGISTER.contains(mSelected))
        regVal = * ((unsigned short*) registerValue(&wRegDumpStruct, mSelected));
    else
        regVal = * ((duint*) registerValue(&wRegDumpStruct, mSelected));
    setRegister(CSP, csp);
    DbgMemWrite(csp, (const unsigned char*)&regVal , sizeof(void*));
}

void RegistersView::onPopAction()
{
    duint csp = (* ((duint*) registerValue(&wRegDumpStruct, CSP)));
    duint newVal;
    DbgMemRead(csp, (unsigned char*)&newVal, sizeof(void*));
    setRegister(CSP, csp + sizeof(void*));
    if(mSEGMENTREGISTER.contains(mSelected))
        newVal &= 0xFFFF;
    setRegister(mSelected, newVal);
}

void RegistersView::onZeroAction()
{
    if(mSETONEZEROTOGGLE.contains(mSelected))
        setRegister(mSelected, 0);
}

void RegistersView::onSetToOneAction()
{
    if(mSETONEZEROTOGGLE.contains(mSelected))
        setRegister(mSelected, 1);
}

void RegistersView::onModifyAction()
{
    if(mMODIFYDISPLAY.contains(mSelected))
        displayEditDialog();
}

void RegistersView::onToggleValueAction()
{
    if(mSETONEZEROTOGGLE.contains(mSelected))
    {
        if(mBOOLDISPLAY.contains(mSelected))
        {
            int value = (int)(* (bool*) registerValue(&wRegDumpStruct, mSelected));
            setRegister(mSelected, value ^ 1);
        }
        else
        {
            bool ok = false;
            dsint val = GetRegStringValueFromValue(mSelected, registerValue(&wRegDumpStruct, mSelected)).toInt(&ok, 16);
            if(ok)
            {
                val++;
                val *= -1;
                setRegister(mSelected, val);
            }
        }
    }
}

void RegistersView::onCopyToClipboardAction()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(GetRegStringValueFromValue(mSelected, registerValue(&wRegDumpStruct, mSelected)));
}

void RegistersView::onCopySymbolToClipboardAction()
{
    if(mLABELDISPLAY.contains(mSelected))
    {
        QClipboard* clipboard = QApplication::clipboard();
        QString symbol = getRegisterLabel(mSelected);
        if(symbol != "")
            clipboard->setText(symbol);
    }
}

void RegistersView::appendRegister(QString & text, REGISTER_NAME reg, const char* name64, const char* name32)
{
    QString symbol;
#ifdef _WIN64
    Q_UNUSED(name32);
    text.append(name64);
#else //x86
    Q_UNUSED(name64);
    text.append(name32);
#endif //_WIN64
    text.append(GetRegStringValueFromValue(reg, registerValue(&wRegDumpStruct, reg)));
    symbol = getRegisterLabel(reg);
    if(symbol != "")
    {
        text.append("     ");
        text.append(symbol);
    }
    text.append("\r\n");
}

void RegistersView::onCopyAllAction()
{
    QString text;
    QClipboard* clipboard;
    // Auto generated code
    appendRegister(text, REGISTER_NAME::CAX, "RAX : ", "EAX : ");
    appendRegister(text, REGISTER_NAME::CCX, "RCX : ", "ECX : ");
    appendRegister(text, REGISTER_NAME::CDX, "RDX : ", "EDX : ");
    appendRegister(text, REGISTER_NAME::CBX, "RBX : ", "EBX : ");
    appendRegister(text, REGISTER_NAME::CDI, "RDI : ", "EDI : ");
    appendRegister(text, REGISTER_NAME::CBP, "RBP : ", "EBP : ");
    appendRegister(text, REGISTER_NAME::CSI, "RSI : ", "ESI : ");
    appendRegister(text, REGISTER_NAME::CSP, "RSP : ", "ESP : ");
#ifdef _WIN64
    appendRegister(text, REGISTER_NAME::R8, "R8  : ", "R8  : ");
    appendRegister(text, REGISTER_NAME::R9, "R9  : ", "R9  : ");
    appendRegister(text, REGISTER_NAME::R10, "R10 : ", "R10 : ");
    appendRegister(text, REGISTER_NAME::R11, "R11 : ", "R11 : ");
    appendRegister(text, REGISTER_NAME::R12, "R12 : ", "R12 : ");
    appendRegister(text, REGISTER_NAME::R13, "R13 : ", "R13 : ");
    appendRegister(text, REGISTER_NAME::R14, "R14 : ", "R14 : ");
    appendRegister(text, REGISTER_NAME::R15, "R15 : ", "R15 : ");
#endif
    appendRegister(text, REGISTER_NAME::CIP, "RIP : ", "EIP : ");
    appendRegister(text, REGISTER_NAME::EFLAGS, "RFLAGS : ", "EFLAGS : ");
    appendRegister(text, REGISTER_NAME::CF, "CF : ", "CF : ");
    appendRegister(text, REGISTER_NAME::PF, "PF : ", "PF : ");
    appendRegister(text, REGISTER_NAME::AF, "AF : ", "AF : ");
    appendRegister(text, REGISTER_NAME::ZF, "ZF : ", "ZF : ");
    appendRegister(text, REGISTER_NAME::SF, "SF : ", "SF : ");
    appendRegister(text, REGISTER_NAME::TF, "TF : ", "TF : ");
    appendRegister(text, REGISTER_NAME::IF, "IF : ", "IF : ");
    appendRegister(text, REGISTER_NAME::DF, "DF : ", "DF : ");
    appendRegister(text, REGISTER_NAME::OF, "OF : ", "OF : ");
    appendRegister(text, REGISTER_NAME::GS, "GS : ", "GS : ");
    appendRegister(text, REGISTER_NAME::FS, "FS : ", "FS : ");
    appendRegister(text, REGISTER_NAME::ES, "ES : ", "ES : ");
    appendRegister(text, REGISTER_NAME::DS, "DS : ", "DS : ");
    appendRegister(text, REGISTER_NAME::CS, "CS : ", "CS : ");
    appendRegister(text, REGISTER_NAME::SS, "SS : ", "SS : ");
    appendRegister(text, REGISTER_NAME::LastError, "LastError : ", "LastError : ");
    appendRegister(text, REGISTER_NAME::DR0, "DR0 : ", "DR0 : ");
    appendRegister(text, REGISTER_NAME::DR1, "DR1 : ", "DR1 : ");
    appendRegister(text, REGISTER_NAME::DR2, "DR2 : ", "DR2 : ");
    appendRegister(text, REGISTER_NAME::DR3, "DR3 : ", "DR3 : ");
    appendRegister(text, REGISTER_NAME::DR6, "DR6 : ", "DR6 : ");
    appendRegister(text, REGISTER_NAME::DR7, "DR7 : ", "DR7 : ");
    appendRegister(text, REGISTER_NAME::x87r0, "x87r0 : ", "x87r0 : ");
    appendRegister(text, REGISTER_NAME::x87r1, "x87r1 : ", "x87r1 : ");
    appendRegister(text, REGISTER_NAME::x87r2, "x87r2 : ", "x87r2 : ");
    appendRegister(text, REGISTER_NAME::x87r3, "x87r3 : ", "x87r3 : ");
    appendRegister(text, REGISTER_NAME::x87r4, "x87r4 : ", "x87r4 : ");
    appendRegister(text, REGISTER_NAME::x87r5, "x87r5 : ", "x87r5 : ");
    appendRegister(text, REGISTER_NAME::x87r6, "x87r6 : ", "x87r6 : ");
    appendRegister(text, REGISTER_NAME::x87r7, "x87r7 : ", "x87r7 : ");
    appendRegister(text, REGISTER_NAME::x87TagWord, "x87TagWord : ", "x87TagWord : ");
    appendRegister(text, REGISTER_NAME::x87ControlWord, "x87ControlWord : ", "x87ControlWord : ");
    appendRegister(text, REGISTER_NAME::x87StatusWord, "x87StatusWord : ", "x87StatusWord : ");
    appendRegister(text, REGISTER_NAME::x87TW_0, "x87TW_0 : ", "x87TW_0 : ");
    appendRegister(text, REGISTER_NAME::x87TW_1, "x87TW_1 : ", "x87TW_1 : ");
    appendRegister(text, REGISTER_NAME::x87TW_2, "x87TW_2 : ", "x87TW_2 : ");
    appendRegister(text, REGISTER_NAME::x87TW_3, "x87TW_3 : ", "x87TW_3 : ");
    appendRegister(text, REGISTER_NAME::x87TW_4, "x87TW_4 : ", "x87TW_4 : ");
    appendRegister(text, REGISTER_NAME::x87TW_5, "x87TW_5 : ", "x87TW_5 : ");
    appendRegister(text, REGISTER_NAME::x87TW_6, "x87TW_6 : ", "x87TW_6 : ");
    appendRegister(text, REGISTER_NAME::x87TW_7, "x87TW_7 : ", "x87TW_7 : ");
    appendRegister(text, REGISTER_NAME::x87SW_B, "x87SW_B : ", "x87SW_B : ");
    appendRegister(text, REGISTER_NAME::x87SW_C3, "x87SW_C3 : ", "x87SW_C3 : ");
    appendRegister(text, REGISTER_NAME::x87SW_TOP, "x87SW_TOP : ", "x87SW_TOP : ");
    appendRegister(text, REGISTER_NAME::x87SW_C2, "x87SW_C2 : ", "x87SW_C2 : ");
    appendRegister(text, REGISTER_NAME::x87SW_C1, "x87SW_C1 : ", "x87SW_C1 : ");
    appendRegister(text, REGISTER_NAME::x87SW_O, "x87SW_O : ", "x87SW_O : ");
    appendRegister(text, REGISTER_NAME::x87SW_IR, "x87SW_IR : ", "x87SW_IR : ");
    appendRegister(text, REGISTER_NAME::x87SW_SF, "x87SW_SF : ", "x87SW_SF : ");
    appendRegister(text, REGISTER_NAME::x87SW_P, "x87SW_P : ", "x87SW_P : ");
    appendRegister(text, REGISTER_NAME::x87SW_U, "x87SW_U : ", "x87SW_U : ");
    appendRegister(text, REGISTER_NAME::x87SW_Z, "x87SW_Z : ", "x87SW_Z : ");
    appendRegister(text, REGISTER_NAME::x87SW_D, "x87SW_D : ", "x87SW_D : ");
    appendRegister(text, REGISTER_NAME::x87SW_I, "x87SW_I : ", "x87SW_I : ");
    appendRegister(text, REGISTER_NAME::x87SW_C0, "x87SW_C0 : ", "x87SW_C0 : ");
    appendRegister(text, REGISTER_NAME::x87CW_IC, "x87CW_IC : ", "x87CW_IC : ");
    appendRegister(text, REGISTER_NAME::x87CW_RC, "x87CW_RC : ", "x87CW_RC : ");
    appendRegister(text, REGISTER_NAME::x87CW_PC, "x87CW_PC : ", "x87CW_PC : ");
    appendRegister(text, REGISTER_NAME::x87CW_IEM, "x87CW_IEM : ", "x87CW_IEM : ");
    appendRegister(text, REGISTER_NAME::x87CW_PM, "x87CW_PM : ", "x87CW_PM : ");
    appendRegister(text, REGISTER_NAME::x87CW_UM, "x87CW_UM : ", "x87CW_UM : ");
    appendRegister(text, REGISTER_NAME::x87CW_OM, "x87CW_OM : ", "x87CW_OM : ");
    appendRegister(text, REGISTER_NAME::x87CW_ZM, "x87CW_ZM : ", "x87CW_ZM : ");
    appendRegister(text, REGISTER_NAME::x87CW_DM, "x87CW_DM : ", "x87CW_DM : ");
    appendRegister(text, REGISTER_NAME::x87CW_IM, "x87CW_IM : ", "x87CW_IM : ");
    appendRegister(text, REGISTER_NAME::MxCsr, "MxCsr : ", "MxCsr : ");
    appendRegister(text, REGISTER_NAME::MxCsr_FZ, "MxCsr_FZ : ", "MxCsr_FZ : ");
    appendRegister(text, REGISTER_NAME::MxCsr_PM, "MxCsr_PM : ", "MxCsr_PM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_UM, "MxCsr_UM : ", "MxCsr_UM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_OM, "MxCsr_OM : ", "MxCsr_OM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_ZM, "MxCsr_ZM : ", "MxCsr_ZM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_IM, "MxCsr_IM : ", "MxCsr_IM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_DM, "MxCsr_DM : ", "MxCsr_DM : ");
    appendRegister(text, REGISTER_NAME::MxCsr_DAZ, "MxCsr_DAZ : ", "MxCsr_DAZ : ");
    appendRegister(text, REGISTER_NAME::MxCsr_PE, "MxCsr_PE : ", "MxCsr_PE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_UE, "MxCsr_UE : ", "MxCsr_UE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_OE, "MxCsr_OE : ", "MxCsr_OE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_ZE, "MxCsr_ZE : ", "MxCsr_ZE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_DE, "MxCsr_DE : ", "MxCsr_DE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_IE, "MxCsr_IE : ", "MxCsr_IE : ");
    appendRegister(text, REGISTER_NAME::MxCsr_RC, "MxCsr_RC : ", "MxCsr_RC : ");
    appendRegister(text, REGISTER_NAME::MM0, "MM0 : ", "MM0 : ");
    appendRegister(text, REGISTER_NAME::MM1, "MM1 : ", "MM1 : ");
    appendRegister(text, REGISTER_NAME::MM2, "MM2 : ", "MM2 : ");
    appendRegister(text, REGISTER_NAME::MM3, "MM3 : ", "MM3 : ");
    appendRegister(text, REGISTER_NAME::MM4, "MM4 : ", "MM4 : ");
    appendRegister(text, REGISTER_NAME::MM5, "MM5 : ", "MM5 : ");
    appendRegister(text, REGISTER_NAME::MM6, "MM6 : ", "MM6 : ");
    appendRegister(text, REGISTER_NAME::MM7, "MM7 : ", "MM7 : ");
    appendRegister(text, REGISTER_NAME::XMM0, "XMM0  : ", "XMM0  : ");
    appendRegister(text, REGISTER_NAME::XMM1, "XMM1  : ", "XMM1  : ");
    appendRegister(text, REGISTER_NAME::XMM2, "XMM2  : ", "XMM2  : ");
    appendRegister(text, REGISTER_NAME::XMM3, "XMM3  : ", "XMM3  : ");
    appendRegister(text, REGISTER_NAME::XMM4, "XMM4  : ", "XMM4  : ");
    appendRegister(text, REGISTER_NAME::XMM5, "XMM5  : ", "XMM5  : ");
    appendRegister(text, REGISTER_NAME::XMM6, "XMM6  : ", "XMM6  : ");
    appendRegister(text, REGISTER_NAME::XMM7, "XMM7  : ", "XMM7  : ");
#ifdef _WIN64
    appendRegister(text, REGISTER_NAME::XMM8, "XMM8  : ", "XMM8  : ");
    appendRegister(text, REGISTER_NAME::XMM9, "XMM9  : ", "XMM9  : ");
    appendRegister(text, REGISTER_NAME::XMM10, "XMM10 : ", "XMM10 : ");
    appendRegister(text, REGISTER_NAME::XMM11, "XMM11 : ", "XMM11 : ");
    appendRegister(text, REGISTER_NAME::XMM12, "XMM12 : ", "XMM12 : ");
    appendRegister(text, REGISTER_NAME::XMM13, "XMM13 : ", "XMM13 : ");
    appendRegister(text, REGISTER_NAME::XMM14, "XMM14 : ", "XMM14 : ");
    appendRegister(text, REGISTER_NAME::XMM15, "XMM15 : ", "XMM15 : ");
    appendRegister(text, REGISTER_NAME::YMM0, "YMM0  : ", "YMM0  : ");
    appendRegister(text, REGISTER_NAME::YMM1, "YMM1  : ", "YMM1  : ");
    appendRegister(text, REGISTER_NAME::YMM2, "YMM2  : ", "YMM2  : ");
    appendRegister(text, REGISTER_NAME::YMM3, "YMM3  : ", "YMM3  : ");
    appendRegister(text, REGISTER_NAME::YMM4, "YMM4  : ", "YMM4  : ");
    appendRegister(text, REGISTER_NAME::YMM5, "YMM5  : ", "YMM5  : ");
    appendRegister(text, REGISTER_NAME::YMM6, "YMM6  : ", "YMM6  : ");
    appendRegister(text, REGISTER_NAME::YMM7, "YMM7  : ", "YMM7  : ");
    appendRegister(text, REGISTER_NAME::YMM8, "YMM8  : ", "YMM8  : ");
    appendRegister(text, REGISTER_NAME::YMM9, "YMM9  : ", "YMM9  : ");
    appendRegister(text, REGISTER_NAME::YMM10, "YMM10 : ", "YMM10 : ");
    appendRegister(text, REGISTER_NAME::YMM11, "YMM11 : ", "YMM11 : ");
    appendRegister(text, REGISTER_NAME::YMM12, "YMM12 : ", "YMM12 : ");
    appendRegister(text, REGISTER_NAME::YMM13, "YMM13 : ", "YMM13 : ");
    appendRegister(text, REGISTER_NAME::YMM14, "YMM14 : ", "YMM14 : ");
    appendRegister(text, REGISTER_NAME::YMM15, "YMM15 : ", "YMM15 : ");
#endif
    // Auto generated code end
    clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

void RegistersView::onFollowInDisassembly()
{
    if(mCANSTOREADDRESS.contains(mSelected))
    {
        QString addr = QString("%1").arg((* ((duint*) registerValue(&wRegDumpStruct, mSelected))), mRegisterPlaces[mSelected].valuesize, 16, QChar('0')).toUpper();
        if(DbgMemIsValidReadPtr((* ((duint*) registerValue(&wRegDumpStruct, mSelected)))))
            DbgCmdExec(QString().sprintf("disasm \"%s\"", addr.toUtf8().constData()).toUtf8().constData());
    }
}

void RegistersView::onFollowInDump()
{
    if(mCANSTOREADDRESS.contains(mSelected))
    {
        QString addr = QString("%1").arg((* ((duint*) registerValue(&wRegDumpStruct, mSelected))), mRegisterPlaces[mSelected].valuesize, 16, QChar('0')).toUpper();
        if(DbgMemIsValidReadPtr((* ((duint*) registerValue(&wRegDumpStruct, mSelected)))))
            DbgCmdExec(QString().sprintf("dump \"%s\"", addr.toUtf8().constData()).toUtf8().constData());
    }
}

void RegistersView::onFollowInStack()
{
    if(mCANSTOREADDRESS.contains(mSelected))
    {
        QString addr = QString("%1").arg((* ((duint*) registerValue(&wRegDumpStruct, mSelected))), mRegisterPlaces[mSelected].valuesize, 16, QChar('0')).toUpper();
        if(DbgMemIsValidReadPtr((* ((duint*) registerValue(&wRegDumpStruct, mSelected)))))
            DbgCmdExec(QString().sprintf("sdump \"%s\"", addr.toUtf8().constData()).toUtf8().constData());
    }
}

void RegistersView::onChangeFPUViewAction()
{
    if(mShowFpu == true)
        ShowFPU(false);
    else
        ShowFPU(true);
}

void RegistersView::displayCustomContextMenuSlot(QPoint pos)
{
    if(!DbgIsDebugging())
        return;
    QMenu wMenu(this);

    if(mSelected != UNKNOWN)
    {
        if(mSETONEZEROTOGGLE.contains(mSelected))
        {
            if((* ((duint*) registerValue(&wRegDumpStruct, mSelected))) >= 1)
                wMenu.addAction(wCM_Zero);
            if((* ((duint*) registerValue(&wRegDumpStruct, mSelected))) == 0)
                wMenu.addAction(wCM_SetToOne);
            wMenu.addAction(wCM_ToggleValue);
        }

        if(mFPUx87_80BITSDISPLAY.contains(mSelected))
        {
            wMenu.addAction(wCM_Incrementx87Stack);
            wMenu.addAction(wCM_Decrementx87Stack);
        }

        if(mINCREMENTDECREMET.contains(mSelected))
        {
            wMenu.addAction(wCM_Increment);
            wMenu.addAction(wCM_Decrement);
            wMenu.addAction(wCM_IncrementPtrSize);
            wMenu.addAction(wCM_DecrementPtrSize);
        }

        if(mGPR.contains(mSelected) || mSEGMENTREGISTER.contains(mSelected))
        {
            wMenu.addAction(wCM_Push);
            wMenu.addAction(wCM_Pop);
        }

        if(mMODIFYDISPLAY.contains(mSelected))
        {
            wMenu.addAction(wCM_Modify);
        }

        if(mCANSTOREADDRESS.contains(mSelected))
        {
            duint addr = (* ((duint*) registerValue(&wRegDumpStruct, mSelected)));
            if(DbgMemIsValidReadPtr(addr))
            {
                wMenu.addAction(wCM_FollowInDump);
                wMenu.addAction(wCM_FollowInDisassembly);
                duint size = 0;
                duint base = DbgMemFindBaseAddr(DbgValFromString("csp"), &size);
                if(addr >= base && addr < base + size)
                    wMenu.addAction(wCM_FollowInStack);
            }
        }

        if(mLABELDISPLAY.contains(mSelected))
        {
            QString symbol = getRegisterLabel(mSelected);
            if(symbol != "")
                wMenu.addAction(wCM_CopySymbolToClipboard);
        }

        wMenu.addAction(wCM_CopyToClipboard);
        wMenu.addAction(wCM_CopyAll);

        wMenu.exec(this->mapToGlobal(pos));
    }
    else
    {
        wMenu.addSeparator();
        wMenu.addAction(wCM_ChangeFPUView);
        wMenu.addAction(wCM_CopyAll);
        wMenu.addSeparator();
        QAction* wHwbpCsp = wMenu.addAction(DIcon("breakpoint.png"), tr("Set Hardware Breakpoint on %1").arg(ArchValue("ESP", "RSP")));
        QAction* wAction = wMenu.exec(this->mapToGlobal(pos));

        if(wAction == wHwbpCsp)
            DbgCmdExec("bphws csp,rw");
    }
}

void RegistersView::setRegister(REGISTER_NAME reg, duint value)
{
    // is register-id known?
    if(mRegisterMapping.contains(reg))
    {
        // map "cax" to "eax" or "rax"
        QString wRegName = mRegisterMapping.constFind(reg).value();

        // flags need to '_' infront
        if(mFlags.contains(reg))
            wRegName = "_" + wRegName;


        // we change the value (so highlight it)
        mRegisterUpdates.insert(reg);
        // tell everything the compiler
        if(mFPU.contains(reg))
            wRegName = "_" + wRegName;

        DbgValToString(wRegName.toUtf8().constData(), value);

        // force repaint
        emit refresh();
    }
}

void RegistersView::debugStateChangedSlot(DBGSTATE state)
{
    if(state == stopped)
    {
        updateRegistersSlot();
    }
}

void RegistersView::repaint()
{
    this->viewport()->repaint();
}

SIZE_T RegistersView::GetSizeRegister(const REGISTER_NAME reg_name)
{
    SIZE_T size;

    if(mUINTDISPLAY.contains(reg_name))
        size = sizeof(duint);
    else if(mUSHORTDISPLAY.contains(reg_name) || mFIELDVALUE.contains(reg_name))
        size = sizeof(unsigned short);
    else if(mDWORDDISPLAY.contains(reg_name))
        size = sizeof(DWORD);
    else if(mBOOLDISPLAY.contains(reg_name))
        size = sizeof(bool);
    else if(mFPUx87_80BITSDISPLAY.contains(reg_name))
        size = 10;
    else if(mFPUMMX.contains(reg_name))
        size = 8;
    else if(mFPUXMM.contains(reg_name))
        size = 16;
    else if(mFPUYMM.contains(reg_name))
        size = 32;
    else if(reg_name == LastError)
        return sizeof(DWORD);
    else
        size = 0;

    return size;
}

int RegistersView::CompareRegisters(const REGISTER_NAME reg_name, REGDUMP* regdump1, REGDUMP* regdump2)
{
    SIZE_T size = GetSizeRegister(reg_name);
    char* reg1_data = registerValue(regdump1, reg_name);
    char* reg2_data = registerValue(regdump2, reg_name);

    if(size != 0)
        return memcmp(reg1_data, reg2_data, size);

    return -1;
}

char* RegistersView::registerValue(const REGDUMP* regd, const REGISTER_NAME reg)
{
    static int null_value = 0;
    // this is probably the most efficient general method to access the values of the struct
    // TODO: add an array with something like: return array[reg].data, this is more fast :-)

    switch(reg)
    {
    case CAX:
        return (char*) &regd->regcontext.cax;
    case CBX:
        return (char*) &regd->regcontext.cbx;
    case CCX:
        return (char*) &regd->regcontext.ccx;
    case CDX:
        return (char*) &regd->regcontext.cdx;
    case CSI:
        return (char*) &regd->regcontext.csi;
    case CDI:
        return (char*) &regd->regcontext.cdi;
    case CBP:
        return (char*) &regd->regcontext.cbp;
    case CSP:
        return (char*) &regd->regcontext.csp;

    case CIP:
        return (char*) &regd->regcontext.cip;
    case EFLAGS:
        return (char*) &regd->regcontext.eflags;
#ifdef _WIN64
    case R8:
        return (char*) &regd->regcontext.r8;
    case R9:
        return (char*) &regd->regcontext.r9;
    case R10:
        return (char*) &regd->regcontext.r10;
    case R11:
        return (char*) &regd->regcontext.r11;
    case R12:
        return (char*) &regd->regcontext.r12;
    case R13:
        return (char*) &regd->regcontext.r13;
    case R14:
        return (char*) &regd->regcontext.r14;
    case R15:
        return (char*) &regd->regcontext.r15;
#endif
    // CF,PF,AF,ZF,SF,TF,IF,DF,OF
    case CF:
        return (char*) &regd->flags.c;
    case PF:
        return (char*) &regd->flags.p;
    case AF:
        return (char*) &regd->flags.a;
    case ZF:
        return (char*) &regd->flags.z;
    case SF:
        return (char*) &regd->flags.s;
    case TF:
        return (char*) &regd->flags.t;
    case IF:
        return (char*) &regd->flags.i;
    case DF:
        return (char*) &regd->flags.d;
    case OF:
        return (char*) &regd->flags.o;

    // GS,FS,ES,DS,CS,SS
    case GS:
        return (char*) &regd->regcontext.gs;
    case FS:
        return (char*) &regd->regcontext.fs;
    case ES:
        return (char*) &regd->regcontext.es;
    case DS:
        return (char*) &regd->regcontext.ds;
    case CS:
        return (char*) &regd->regcontext.cs;
    case SS:
        return (char*) &regd->regcontext.ss;

    case LastError:
        return (char*) &regd->lastError;

    case DR0:
        return (char*) &regd->regcontext.dr0;
    case DR1:
        return (char*) &regd->regcontext.dr1;
    case DR2:
        return (char*) &regd->regcontext.dr2;
    case DR3:
        return (char*) &regd->regcontext.dr3;
    case DR6:
        return (char*) &regd->regcontext.dr6;
    case DR7:
        return (char*) &regd->regcontext.dr7;

    case MM0:
        return (char*) &regd->mmx[0];
    case MM1:
        return (char*) &regd->mmx[1];
    case MM2:
        return (char*) &regd->mmx[2];
    case MM3:
        return (char*) &regd->mmx[3];
    case MM4:
        return (char*) &regd->mmx[4];
    case MM5:
        return (char*) &regd->mmx[5];
    case MM6:
        return (char*) &regd->mmx[6];
    case MM7:
        return (char*) &regd->mmx[7];

    case x87r0:
        return (char*) &regd->x87FPURegisters[0];
    case x87r1:
        return (char*) &regd->x87FPURegisters[1];
    case x87r2:
        return (char*) &regd->x87FPURegisters[2];
    case x87r3:
        return (char*) &regd->x87FPURegisters[3];
    case x87r4:
        return (char*) &regd->x87FPURegisters[4];
    case x87r5:
        return (char*) &regd->x87FPURegisters[5];
    case x87r6:
        return (char*) &regd->x87FPURegisters[6];
    case x87r7:
        return (char*) &regd->x87FPURegisters[7];

    case x87TagWord:
        return (char*) &regd->regcontext.x87fpu.TagWord;

    case x87ControlWord:
        return (char*) &regd->regcontext.x87fpu.ControlWord;

    case x87TW_0:
        return (char*) &regd->x87FPURegisters[0].tag;
    case x87TW_1:
        return (char*) &regd->x87FPURegisters[1].tag;
    case x87TW_2:
        return (char*) &regd->x87FPURegisters[2].tag;
    case x87TW_3:
        return (char*) &regd->x87FPURegisters[3].tag;
    case x87TW_4:
        return (char*) &regd->x87FPURegisters[4].tag;
    case x87TW_5:
        return (char*) &regd->x87FPURegisters[5].tag;
    case x87TW_6:
        return (char*) &regd->x87FPURegisters[6].tag;
    case x87TW_7:
        return (char*) &regd->x87FPURegisters[7].tag;

    case x87CW_IC:
        return (char*) &regd->x87ControlWordFields.IC;
    case x87CW_IEM:
        return (char*) &regd->x87ControlWordFields.IEM;
    case x87CW_PM:
        return (char*) &regd->x87ControlWordFields.PM;
    case x87CW_UM:
        return (char*) &regd->x87ControlWordFields.UM;
    case x87CW_OM:
        return (char*) &regd->x87ControlWordFields.OM;
    case x87CW_ZM:
        return (char*) &regd->x87ControlWordFields.ZM;
    case x87CW_DM:
        return (char*) &regd->x87ControlWordFields.DM;
    case x87CW_IM:
        return (char*) &regd->x87ControlWordFields.IM;
    case x87CW_RC:
        return (char*) &regd->x87ControlWordFields.RC;
    case x87CW_PC:
        return (char*) &regd->x87ControlWordFields.PC;

    case x87StatusWord:
        return (char*) &regd->regcontext.x87fpu.StatusWord;

    case x87SW_B:
        return (char*) &regd->x87StatusWordFields.B;
    case x87SW_C3:
        return (char*) &regd->x87StatusWordFields.C3;
    case x87SW_C2:
        return (char*) &regd->x87StatusWordFields.C2;
    case x87SW_C1:
        return (char*) &regd->x87StatusWordFields.C1;
    case x87SW_O:
        return (char*) &regd->x87StatusWordFields.O;
    case x87SW_IR:
        return (char*) &regd->x87StatusWordFields.IR;
    case x87SW_SF:
        return (char*) &regd->x87StatusWordFields.SF;
    case x87SW_P:
        return (char*) &regd->x87StatusWordFields.P;
    case x87SW_U:
        return (char*) &regd->x87StatusWordFields.U;
    case x87SW_Z:
        return (char*) &regd->x87StatusWordFields.Z;
    case x87SW_D:
        return (char*) &regd->x87StatusWordFields.D;
    case x87SW_I:
        return (char*) &regd->x87StatusWordFields.I;
    case x87SW_C0:
        return (char*) &regd->x87StatusWordFields.C0;
    case x87SW_TOP:
        return (char*) &regd->x87StatusWordFields.TOP;

    case MxCsr:
        return (char*) &regd->regcontext.MxCsr;

    case MxCsr_FZ:
        return (char*) &regd->MxCsrFields.FZ;
    case MxCsr_PM:
        return (char*) &regd->MxCsrFields.PM;
    case MxCsr_UM:
        return (char*) &regd->MxCsrFields.UM;
    case MxCsr_OM:
        return (char*) &regd->MxCsrFields.OM;
    case MxCsr_ZM:
        return (char*) &regd->MxCsrFields.ZM;
    case MxCsr_IM:
        return (char*) &regd->MxCsrFields.IM;
    case MxCsr_DM:
        return (char*) &regd->MxCsrFields.DM;
    case MxCsr_DAZ:
        return (char*) &regd->MxCsrFields.DAZ;
    case MxCsr_PE:
        return (char*) &regd->MxCsrFields.PE;
    case MxCsr_UE:
        return (char*) &regd->MxCsrFields.UE;
    case MxCsr_OE:
        return (char*) &regd->MxCsrFields.OE;
    case MxCsr_ZE:
        return (char*) &regd->MxCsrFields.ZE;
    case MxCsr_DE:
        return (char*) &regd->MxCsrFields.DE;
    case MxCsr_IE:
        return (char*) &regd->MxCsrFields.IE;
    case MxCsr_RC:
        return (char*) &regd->MxCsrFields.RC;

    case XMM0:
        return (char*) &regd->regcontext.XmmRegisters[0];
    case XMM1:
        return (char*) &regd->regcontext.XmmRegisters[1];
    case XMM2:
        return (char*) &regd->regcontext.XmmRegisters[2];
    case XMM3:
        return (char*) &regd->regcontext.XmmRegisters[3];
    case XMM4:
        return (char*) &regd->regcontext.XmmRegisters[4];
    case XMM5:
        return (char*) &regd->regcontext.XmmRegisters[5];
    case XMM6:
        return (char*) &regd->regcontext.XmmRegisters[6];
    case XMM7:
        return (char*) &regd->regcontext.XmmRegisters[7];
#ifdef _WIN64
    case XMM8:
        return (char*) &regd->regcontext.XmmRegisters[8];
    case XMM9:
        return (char*) &regd->regcontext.XmmRegisters[9];
    case XMM10:
        return (char*) &regd->regcontext.XmmRegisters[10];
    case XMM11:
        return (char*) &regd->regcontext.XmmRegisters[11];
    case XMM12:
        return (char*) &regd->regcontext.XmmRegisters[12];
    case XMM13:
        return (char*) &regd->regcontext.XmmRegisters[13];
    case XMM14:
        return (char*) &regd->regcontext.XmmRegisters[14];
    case XMM15:
        return (char*) &regd->regcontext.XmmRegisters[15];
#endif //_WIN64

    case YMM0:
        return (char*) &regd->regcontext.YmmRegisters[0];
    case YMM1:
        return (char*) &regd->regcontext.YmmRegisters[1];
    case YMM2:
        return (char*) &regd->regcontext.YmmRegisters[2];
    case YMM3:
        return (char*) &regd->regcontext.YmmRegisters[3];
    case YMM4:
        return (char*) &regd->regcontext.YmmRegisters[4];
    case YMM5:
        return (char*) &regd->regcontext.YmmRegisters[5];
    case YMM6:
        return (char*) &regd->regcontext.YmmRegisters[6];
    case YMM7:
        return (char*) &regd->regcontext.YmmRegisters[7];
#ifdef _WIN64
    case YMM8:
        return (char*) &regd->regcontext.YmmRegisters[8];
    case YMM9:
        return (char*) &regd->regcontext.YmmRegisters[9];
    case YMM10:
        return (char*) &regd->regcontext.YmmRegisters[10];
    case YMM11:
        return (char*) &regd->regcontext.YmmRegisters[11];
    case YMM12:
        return (char*) &regd->regcontext.YmmRegisters[12];
    case YMM13:
        return (char*) &regd->regcontext.YmmRegisters[13];
    case YMM14:
        return (char*) &regd->regcontext.YmmRegisters[14];
    case YMM15:
        return (char*) &regd->regcontext.YmmRegisters[15];
#endif //_WIN64
    }

    return (char*) &null_value;
}

void RegistersView::setRegisters(REGDUMP* reg)
{
    // tests if new-register-value == old-register-value holds
    if(mCip != reg->regcontext.cip) //CIP changed
    {
        wCipRegDumpStruct = wRegDumpStruct;
        mRegisterUpdates.clear();
        mCip = reg->regcontext.cip;
    }

    // iterate all ids (CAX, CBX, ...)
    for(auto itr = mRegisterMapping.begin(); itr != mRegisterMapping.end(); itr++)
    {
        if(CompareRegisters(itr.key(), reg, &wCipRegDumpStruct) != 0)
            mRegisterUpdates.insert(itr.key());
        else if(mRegisterUpdates.contains(itr.key())) //registers are equal
            mRegisterUpdates.remove(itr.key());
    }

    // now we can save the values
    wRegDumpStruct = (*reg);

    if(mCip != reg->regcontext.cip)
        wCipRegDumpStruct = wRegDumpStruct;

    // force repaint
    emit refresh();
}
