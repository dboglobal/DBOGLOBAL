CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11215;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 1;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 12216;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 1;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 12215;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 1;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 12214;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 8;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckInWorld
			{
				widx = 192000;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "0;";
			nolnk = 5;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckInWorld
			{
				widx = 28000;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;3;2;1;";

			CDboTSActSendSvrEvt
			{
				id = 12520;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "252;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 12510;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 252;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 252;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100183;
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
			cancellnk = 252;
			cid = 1;
			dt = 0;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "6;8;7;";
			dg = 1100176;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 252;
			cid = 2;
			dt = 0;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100177;
			gt = 0;
			oi = 2413109;
		}
	}
}

