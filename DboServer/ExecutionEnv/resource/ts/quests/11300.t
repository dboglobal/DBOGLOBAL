CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11300;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;4;3;2;1;5;";

			CDboTSActSendSvrEvt
			{
				id = 13010;
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
			prelnk = "6;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
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
				id = 13000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100227;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100226;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100224;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100225;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContNarration
		{
			cancellnk = 6;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100223;
			gt = 0;
			oi = 7511101;
		}
	}
}

