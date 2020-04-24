CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 407;
	title = 40502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 20;
			prelnk = "0;";
			nolnk = 1;
			rm = 0;
			yeslnk = 254;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 634;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 13;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 634;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "20;51;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 51;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 40522;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "20;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40507;
			nolnk = 255;
			rm = 0;
			yeslnk = 20;

			CDboTSCheckObjItem
			{
				tid = 405;
			}
			CDboTSClickObject
			{
				objidx = "472;";
				widx = 1;
			}
		}
	}
}

