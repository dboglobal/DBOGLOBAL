CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11131;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 11470;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 3;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100932;
				gtype = 0;
				area = 1100807;
				goal = 1100931;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100930;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 2911101;
				cnt0 = 2;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 11450;
			}
		}
	}
}

