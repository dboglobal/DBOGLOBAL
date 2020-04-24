CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11328;
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
				cont = 1100276;
				gtype = -1;
				area = 1100262;
				goal = 1100275;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100274;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 7111404;
				cnt0 = 4;
				cnt2 = 8;
				ectype = -1;
				etype = 0;
				idx1 = 7111405;
				taid = 2;
				esctype = 0;
				idx2 = 4031105;
			}
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
				id = 13306;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
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

