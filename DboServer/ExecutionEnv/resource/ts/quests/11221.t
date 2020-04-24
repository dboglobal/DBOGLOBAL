CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11221;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "11;";
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
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 12340;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 254;
			prelnk = "10;10;8;9;";

			CDboTSActSendSvrEvt
			{
				id = 12350;
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
			cancellnk = 11;
			cid = 8;
			dt = 0;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "0;";
			dg = 1100168;
			gt = 0;
			oi = 2761102;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 9;
			dt = 0;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100169;
			gt = 0;
			oi = 5551103;
		}
		CDboTSContNarration
		{
			cancellnk = 11;
			cid = 10;
			dt = 0;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "9;";
			dg = 1100170;
			gt = 0;
			oi = 2761102;
		}
	}
}

