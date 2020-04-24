CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11214;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "14;";

			CDboTSActSendSvrEvt
			{
				id = 12471;
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
			cid = 14;
			prelnk = "12;";
			nolnk = 255;
			rm = 0;
			yeslnk = 15;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 7;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 1;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100173;
			gt = 0;
			oi = 7011101;
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 1;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100214;
			gt = 0;
			oi = 7011101;
		}
		CDboTSContGCond
		{
			cid = 16;
			prelnk = "15;";
			nolnk = 255;
			rm = 0;
			yeslnk = 17;

			CDboTSRcvSvrEvt
			{
				id = 12480;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100175;
			gt = 0;
			oi = 5601101;
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 14;
			prelnk = "7;6;10;11;11;";

			CDboTSActSendSvrEvt
			{
				id = 12470;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 6;
			prelnk = "0;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7011100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100209;
				gtype = -1;
				area = 1100021;
				goal = 1100208;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 0;
				taid = 2;
				title = 1100207;
			}
		}
		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 254;
			prelnk = "17;17;";

			CDboTSActSendSvrEvt
			{
				id = 12490;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 12;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 1;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1100172;
			gt = 0;
			oi = 7011101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 17;
			dt = 0;
			lilnk = 255;
			oklnk = 18;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "16;";
			dg = 1100182;
			gt = 0;
			oi = 7511101;
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
			prelnk = "18;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 13;

			CDboTSRcvSvrEvt
			{
				id = 12460;
			}
		}
	}
}

