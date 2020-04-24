CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13100;
	title = 1101280;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 22;
			prelnk = "25;13;12;11;10;9;8;";
			nolnk = 255;
			rm = 0;
			yeslnk = 24;

			CDboTSClickNPC
			{
				npcidx = 7611210;
			}
		}
		CDboTSContEnd
		{
			cid = 211;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 22;
			cid = 25;
			lilnk = 255;
			ndesc0 = 1101240;
			uspt = 0;
			desc = 1101280;
			type = 0;
			ust = 1;
			idx = 7611209;
			nid1 = 18;
			prelnk = "24;24;";
			ndesc1 = 1101241;
			nid0 = 14;
		}
		CDboTSContNarration
		{
			cancellnk = 25;
			cid = 24;
			dt = 2;
			lilnk = 255;
			oklnk = 25;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "22;";
			dg = 1101239;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContGAct
		{
			cid = 20;
			elnk = 255;
			nextlnk = 210;
			prelnk = "18;19;19;";

			CDboTSActSendSvrEvt
			{
				id = 647;
				tblidx = 70100;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 210;
			prelnk = "20;16;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 210;
			prelnk = "15;14;15;";

			CDboTSActSendSvrEvt
			{
				id = 642;
				tblidx = 70100;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 20;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "18;";
			dg = 1101254;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContUsrSel
		{
			cancellnk = 22;
			cid = 13;
			lilnk = 255;
			ndesc0 = 1101240;
			uspt = 1;
			desc = 1101280;
			type = 0;
			ust = 1;
			idx = 7611209;
			nid1 = 18;
			prelnk = "12;";
			ndesc1 = 1101241;
			nid0 = 14;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 211;
			rm = 0;
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 641;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 15;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1101243;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 20;
			cid = 18;
			dt = 2;
			lilnk = 255;
			oklnk = 19;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "25;13;";
			dg = 1101253;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 16;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "25;13;";
			dg = 1101242;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 12;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1101239;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 11;
			dt = 0;
			lilnk = 255;
			oklnk = 12;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1101238;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1101237;
			gt = 0;
			oi = 7611209;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1101236;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101235;
			gt = 0;
			oi = 7611209;
		}
	}
}

