CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1425;
	title = 142502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 4;
				osh_sh = 1;
				widx = 2;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 15000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 4;
				osh_sh = 0;
				widx = 2;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 1;
			prelnk = "12;";

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
			stdiag = 142507;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckObjItem
			{
				tid = 1425;
			}
			CDboTSClickObject
			{
				objidx = "4;";
				widx = 2;
			}
		}
		CDboTSContGCond
		{
			cid = 12;
			prelnk = "11;";
			nolnk = 254;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "13;";

			CDboTSActPCConv
			{
				taid = 2;
				tblidx = 142518;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 844;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;12;";
			type = 1;
		}
	}
}

