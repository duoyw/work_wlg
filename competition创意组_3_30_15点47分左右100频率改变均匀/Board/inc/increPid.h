#ifndef _INCREPID_H_
#define _INCREPID_H_



int16 UpdataPidIcerment(Dof_PID_t* pPID,float Sample_NewRaw);
void UpdataPidSet(Dof_PID_t* pPID, float Set_New);
void UpdataMotoPid(float newErr);       //ÔöÁ¿Ê½PID

#endif