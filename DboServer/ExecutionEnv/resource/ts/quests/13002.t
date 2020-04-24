CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13002;
	title = 1101150;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 625;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 103;
			cid = 102;
			lilnk = 255;
			ndesc0 = 1101171;
			uspt = 3;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 17;
			prelnk = "11;11;";
			ndesc1 = 1101172;
			nid0 = 13;
		}
		CDboTSContGCond
		{
			cid = 103;
			prelnk = "101;102;";
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSClickNPC
			{
				npcidx = 7031201;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 102;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 102;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "103;";
			dg = 1101170;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 254;
			prelnk = "20;20;19;18;17;";

			CDboTSActSendSvrEvt
			{
				id = 629;
				tblidx = 70001;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 104;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;13;";

			CDboTSActSendSvrEvt
			{
				id = 626;
				tblidx = 70001;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 103;
			cid = 101;
			lilnk = 255;
			ndesc0 = 1101171;
			uspt = 2;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 17;
			prelnk = "8;9;10;10;";
			ndesc1 = 1101172;
			nid0 = 13;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "105;104;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 105;
			cid = 20;
			dt = 2;
			lilnk = 255;
			oklnk = 105;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "19;";
			dg = 1101180;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 105;
			cid = 17;
			dt = 2;
			lilnk = 255;
			oklnk = 18;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "102;101;";
			dg = 1101177;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 105;
			cid = 18;
			dt = 2;
			lilnk = 255;
			oklnk = 19;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "17;";
			dg = 1101178;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 105;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "18;";
			dg = 1101179;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 104;
			cid = 13;
			dt = 2;
			lilnk = 255;
			oklnk = 104;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "102;101;";
			dg = 1101173;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101168;
			gt = 0;
			oi = 7031101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1101169;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1101170;
			gt = 0;
			oi = 7031101;
		}
	}
}

