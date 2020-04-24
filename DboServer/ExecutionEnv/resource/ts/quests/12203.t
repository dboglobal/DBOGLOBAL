CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12203;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActSendSvrEvt
			{
				id = 22130;
				tblidx = -1;
				stradius = 0;
				stype = 0;
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
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 22120;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101069;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101071;
			gt = 0;
			oi = 7511101;
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
				id = 22100;
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
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActRegQInfo
			{
				cont = 1101112;
				gtype = -1;
				area = 1101104;
				goal = 1101111;
				grade = -1;
				rwd = 255;
				sort = -1;
				stype = 4;
				taid = 1;
				title = 1101110;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 9;
				eitype = 0;
				idx0 = 4613102;
				cnt0 = 3;
				cnt2 = 10;
				ectype = -1;
				etype = 0;
				idx1 = 7114211;
				taid = 2;
				esctype = 0;
				idx2 = 4652105;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;2;1;3;";

			CDboTSActSendSvrEvt
			{
				id = 22110;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 4;
			cid = 2;
			lilnk = 255;
			ndesc0 = 1101070;
			uspt = 3;
			desc = -1;
			nid0 = 3;
			type = -1;
			ust = 1;
			idx = -1;
			prelnk = "1;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "8;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
	}
}

