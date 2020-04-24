CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12131;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 1006;
				tblidx = 7511107;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 1005;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 1004;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 36911100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

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
			elnk = 253;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActWorldPlayScript
			{
				sid = 12131;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;";
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
				id = 1002;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1100988;
				gtype = -1;
				area = 1100972;
				goal = 1100987;
				grade = -1;
				rwd = 255;
				sort = -1;
				stype = 4;
				taid = 1;
				title = 1100986;
			}
		}
	}
}

