CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11220;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 25;
			prelnk = "0;";

			CDboTSActRegQInfo
			{
				cont = 1100203;
				gtype = 3;
				area = 1100195;
				goal = 1100202;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100201;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 4;
				eitype = 0;
				idx0 = 7114302;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 7114300;
				taid = 2;
				esctype = 0;
				idx2 = 4531100;
			}
		}
		CDboTSContGAct
		{
			cid = 26;
			elnk = 255;
			nextlnk = 254;
			prelnk = "25;";

			CDboTSActSToCEvt
			{
				apptype = 0;
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
				id = 12346;
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
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 12320;
			}
		}
		CDboTSContGCond
		{
			cid = 25;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 26;

			CDboTSRcvSvrEvt
			{
				id = 12343;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "26;";
			type = 1;
		}
	}
}

