CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6016;
	title = 601602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;7;";

			CDboTSActRegQInfo
			{
				cont = 601608;
				gtype = 1;
				area = 601601;
				goal = 601604;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 601605;
				stype = 8;
				taid = 1;
				title = 601602;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "1;";
			nolnk = 2;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99080;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 2;
			prelnk = "6;";

			CDboTSActItem
			{
				iidx0 = 99080;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99081;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "100;";
			nolnk = 5;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99081;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 601607;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6015;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 5;
			prelnk = "4;3;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 601608;
			gtype = 1;
			oklnk = 6;
			area = 601601;
			goal = 601604;
			sort = 601605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 601602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 601614;
			nextlnk = 3;
			rwdexp = 0;
			rwdtbl = 601601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332301;";
			}
		}
	}
}

