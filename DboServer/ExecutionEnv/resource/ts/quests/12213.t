CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12213;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1101131;
				gtype = -1;
				area = 1101104;
				goal = 1101130;
				grade = -1;
				rwd = 255;
				sort = -1;
				stype = 4;
				taid = 2;
				title = 1101129;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 4613102;
				cnt0 = 2;
				cnt2 = 8;
				ectype = -1;
				etype = 0;
				idx1 = 7114211;
				taid = 1;
				esctype = 0;
				idx2 = 7115100;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActSendSvrEvt
			{
				id = 22370;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
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
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 22360;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

