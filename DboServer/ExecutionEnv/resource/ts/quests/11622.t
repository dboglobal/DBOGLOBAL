CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11622;
	title = 1100664;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100667;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 2951203;
			}
			CDboTSCheckClrQst
			{
				and = "11620;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;2;3;3;";

			CDboTSActSendSvrEvt
			{
				id = 16730;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "2;";
			dg = 1100669;
			gt = 0;
			oi = 2951203;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 3;
			prelnk = "1;";
			dg = 1100658;
			gt = 0;
			oi = 2741101;
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100668;
			gt = 0;
			oi = -1;
		}
	}
}

