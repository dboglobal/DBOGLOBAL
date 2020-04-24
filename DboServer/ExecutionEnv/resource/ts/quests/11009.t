CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11009;
	title = 1000139;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 250;
			elnk = 255;
			nextlnk = 254;
			prelnk = "80;83;82;50;51;60;47;46;40;37;31;36;30;27;26;35;25;23;21;22;20;15;7;11;6;10;5;1;81;81;";

			CDboTSActSendSvrEvt
			{
				id = 1100901;
				tblidx = -1;
				stype = 2;
				taid = 1;
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
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 60;
			lilnk = 255;
			ndesc0 = 1000158;
			uspt = 1;
			desc = -1;
			nid0 = 82;
			type = -1;
			ust = 1;
			idx = -1;
			prelnk = "51;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1000140;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "7511101;";
			}
			CDboTSCheckClrQst
			{
				and = "11008;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 83;
			dt = 1;
			lilnk = 255;
			oklnk = 80;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "82;";
			dg = 1000159;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 82;
			dt = 0;
			lilnk = 255;
			oklnk = 83;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "60;";
			dg = 1000158;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 81;
			dt = 1;
			lilnk = 255;
			oklnk = 250;
			ot = 0;
			pt = 2;
			mlt = 5000;
			os = 0;
			prelnk = "80;";
			dg = 1000163;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 80;
			dt = 1;
			lilnk = 255;
			oklnk = 81;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "83;47;";
			dg = 1000162;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 51;
			dt = 1;
			lilnk = 255;
			oklnk = 60;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "50;";
			dg = 1000157;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 30;
			lilnk = 255;
			ndesc0 = 1000154;
			uspt = 1;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 35;
			prelnk = "27;";
			ndesc1 = 1000152;
			nid0 = 31;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 50;
			dt = 0;
			lilnk = 255;
			oklnk = 51;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "40;";
			dg = 1000156;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 47;
			dt = 1;
			lilnk = 255;
			oklnk = 80;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "46;";
			dg = 1000161;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 46;
			dt = 0;
			lilnk = 255;
			oklnk = 47;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "40;";
			dg = 1000160;
			gt = 0;
			oi = -1;
		}
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 40;
			lilnk = 255;
			ndesc0 = 1000160;
			uspt = 1;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 50;
			prelnk = "37;";
			ndesc1 = 1000156;
			nid0 = 46;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 31;
			dt = 0;
			lilnk = 255;
			oklnk = 37;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "36;30;";
			dg = 1000154;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 37;
			dt = 1;
			lilnk = 255;
			oklnk = 40;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "31;";
			dg = 1000155;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 36;
			dt = 1;
			lilnk = 255;
			oklnk = 31;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "35;";
			dg = 1000153;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 27;
			dt = 1;
			lilnk = 255;
			oklnk = 30;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "26;";
			dg = 1000151;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 25;
			lilnk = 255;
			ndesc0 = 1000150;
			uspt = 1;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 35;
			prelnk = "23;";
			ndesc1 = 1000152;
			nid0 = 26;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 35;
			dt = 0;
			lilnk = 255;
			oklnk = 36;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "30;25;";
			dg = 1000152;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 26;
			dt = 0;
			lilnk = 255;
			oklnk = 27;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "25;";
			dg = 1000150;
			gt = 0;
			oi = -1;
		}
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 20;
			lilnk = 255;
			ndesc0 = 1000147;
			uspt = 1;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 22;
			prelnk = "15;";
			ndesc1 = 1000148;
			nid0 = 21;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 23;
			dt = 1;
			lilnk = 255;
			oklnk = 25;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "22;21;";
			dg = 1000149;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 22;
			dt = 0;
			lilnk = 255;
			oklnk = 23;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "20;";
			dg = 1000148;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 21;
			dt = 0;
			lilnk = 255;
			oklnk = 23;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "20;";
			dg = 1000147;
			gt = 0;
			oi = -1;
		}
		CDboTSContUsrSel
		{
			cancellnk = 250;
			cid = 5;
			lilnk = 255;
			ndesc0 = 1000142;
			uspt = 0;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 10;
			prelnk = "1;";
			ndesc1 = 1000144;
			nid0 = 6;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 15;
			dt = 1;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "11;7;";
			dg = 1000146;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 11;
			dt = 1;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "10;";
			dg = 1000145;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 7;
			dt = 1;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 1;
			mlt = 5000;
			os = 0;
			prelnk = "6;";
			dg = 1000143;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "5;";
			dg = 1000144;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 2;
			pt = 1;
			mlt = 1000;
			os = 0;
			prelnk = "5;";
			dg = 1000142;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "250;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 250;
			cid = 1;
			dt = 1;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 0;
			mlt = 5000;
			os = 0;
			prelnk = "0;";
			dg = 1000141;
			gt = 0;
			oi = 7511101;
		}
	}
}

