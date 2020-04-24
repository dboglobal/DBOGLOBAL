CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11108;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;1;2;";

			CDboTSActSendSvrEvt
			{
				id = 11140;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 11170;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
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
				id = 11160;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
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
			cancellnk = 3;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100844;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 1;
			dt = 0;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100843;
			gt = 0;
			oi = 7511101;
		}
	}
}

