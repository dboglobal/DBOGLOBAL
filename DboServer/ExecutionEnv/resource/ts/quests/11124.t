CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11124;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActSendSvrEvt
			{
				id = 11380;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 7111400;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112415;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100907;
				gtype = 1;
				area = 1100807;
				goal = 1100906;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100905;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "0;";
			nolnk = 1;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
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
			yeslnk = 240;

			CDboTSRcvSvrEvt
			{
				id = 11360;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "241;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100907;
				gtype = 1;
				area = 1100807;
				goal = 1100906;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100905;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 7111400;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112409;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

