CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11429;
	title = 1100266;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "22;";
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
			yeslnk = 19;

			CDboTSRcvSvrEvt
			{
				id = 14580;
			}
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 254;
			prelnk = "21;21;20;19;";

			CDboTSActSendSvrEvt
			{
				id = 14590;
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
			cancellnk = 22;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 2;
			prelnk = "0;";
			dg = 1100355;
			gt = 0;
			oi = 3511102;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 20;
			dt = 2;
			lilnk = 255;
			oklnk = 21;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "19;";
			dg = 1100356;
			gt = 0;
			oi = 2931102;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 21;
			dt = 2;
			lilnk = 255;
			oklnk = 22;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 3;
			prelnk = "20;";
			dg = 1100357;
			gt = 0;
			oi = 2761101;
		}
	}
}

