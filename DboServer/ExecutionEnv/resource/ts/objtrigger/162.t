CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 162;
	title = 16202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 51;
			prelnk = "21;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 16219;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "11;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 19;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 254;
			prelnk = "22;16;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 10000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;51;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 21;
			elnk = 255;
			nextlnk = 22;
			prelnk = "11;";
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 51;
			prelnk = "15;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 16218;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 2;
			cid = 11;
			cid1 = 21;
			prelnk = "3;";
			bid0 = 1;
			cid0 = 15;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "1.000000";
				sbi = 1;
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
			nextlnk = 11;
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "1;";
				widx = 2;
			}
			CDboTSCheckObjItem
			{
				tid = 162;
			}
		}
	}
}

