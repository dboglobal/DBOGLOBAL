CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11316;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;2;1;3;";

			CDboTSActSendSvrEvt
			{
				id = 13320;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100279;
				gtype = -1;
				area = 1100262;
				goal = 1100278;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100277;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 4611202;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 4612102;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 1;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100241;
			gt = 0;
			oi = 4612112;
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
				id = 13310;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100240;
			gt = 0;
			oi = 4611212;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 1;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100239;
			gt = 0;
			oi = 4612112;
		}
	}
}

