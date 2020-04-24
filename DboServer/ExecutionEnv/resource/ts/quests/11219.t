CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11219;
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
				id = 12345;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
			}
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
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
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
				idx0 = 7111403;
				cnt0 = 2;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5214100;
				taid = 2;
				esctype = 0;
				idx2 = -1;
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
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 12322;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSRcvSvrEvt
			{
				id = 12342;
			}
		}
	}
}

