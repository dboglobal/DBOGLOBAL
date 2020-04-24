CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11611;
	title = 1100422;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 20;
			elnk = 255;
			nextlnk = 254;
			prelnk = "19;";

			CDboTSActDirIndicator
			{
				dtype = 2;
				self = 1;
				y = "20.370001";
				stype = 1;
				taid = 2;
				x = "782.799988";
				z = "-1098.660034";
			}
			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1100444;
			}
		}
		CDboTSContGAct
		{
			cid = 19;
			elnk = 255;
			nextlnk = 20;
			prelnk = "18;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 19;
			prelnk = "16;15;14;13;12;11;10;9;8;7;6;5;4;3;2;1;17;17;";

			CDboTSActSendSvrEvt
			{
				id = 16210;
				tblidx = 7511101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100430;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 16;
			dt = 2;
			lilnk = 255;
			oklnk = 17;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "15;";
			dg = 1100443;
			gt = 0;
			oi = -1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100422;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 11612;
			}
			CDboTSClickNPC
			{
				npcidx = 7511101;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 15;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1100436;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 17;
			dt = 2;
			lilnk = 255;
			oklnk = 18;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "16;";
			dg = 1100437;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 13;
			dt = 2;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "12;";
			dg = 1100434;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1100435;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 12;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100432;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 12;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1100433;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "20;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100431;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100428;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100429;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100427;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100426;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100425;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100424;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100423;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 18;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100422;
			gt = 0;
			oi = 7511101;
		}
	}
}

