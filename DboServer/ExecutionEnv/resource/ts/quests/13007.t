CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13007;
	title = 1300002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "39;39;38;37;36;35;34;33;32;31;";

			CDboTSActSendSvrEvt
			{
				id = 676;
				tblidx = 70003;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 31;

			CDboTSRcvSvrEvt
			{
				id = 638;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 39;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "38;";
			dg = 1101199;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 31;
			dt = 2;
			lilnk = 255;
			oklnk = 32;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101191;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 32;
			dt = 2;
			lilnk = 255;
			oklnk = 33;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "31;";
			dg = 1101192;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 38;
			dt = 2;
			lilnk = 255;
			oklnk = 39;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "37;";
			dg = 1101198;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 33;
			dt = 2;
			lilnk = 255;
			oklnk = 34;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "32;";
			dg = 1101193;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 35;
			dt = 2;
			lilnk = 255;
			oklnk = 36;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "34;";
			dg = 1101195;
			gt = 0;
			oi = 2731101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 34;
			dt = 2;
			lilnk = 255;
			oklnk = 35;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "33;";
			dg = 1101194;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 36;
			dt = 2;
			lilnk = 255;
			oklnk = 37;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "35;";
			dg = 1101196;
			gt = 0;
			oi = 1381101;
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 37;
			dt = 2;
			lilnk = 255;
			oklnk = 38;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "36;";
			dg = 1101197;
			gt = 0;
			oi = 2731101;
		}
	}
}

