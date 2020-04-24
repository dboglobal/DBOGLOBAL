CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6251;
	title = 625102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 253;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 30;
				osh_sh = 0;
				widx = 700000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

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
			nolnk = 253;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "6;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1174;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 625107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 6251;
			}
			CDboTSClickObject
			{
				objidx = "30;";
				widx = 700000;
			}
		}
	}
}

