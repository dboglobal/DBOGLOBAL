CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11200;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 12000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;1;2;3;4;6;6;";

			CDboTSActSendSvrEvt
			{
				id = 12010;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 6;
			dt = 0;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100155;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 0;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100154;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 4;
			dt = 0;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100153;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100152;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100151;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 1;
			dt = 0;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100150;
			gt = 0;
			oi = 7511101;
		}
	}
}

