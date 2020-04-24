CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2677;
	title = 267702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 267709;
				ctype = 1;
				idx = 1502201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 267708;
				gtype = 1;
				area = 267701;
				goal = 267704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 267705;
				stype = 2;
				taid = 1;
				title = 267702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1502201;";
			}
			CDboTSCheckClrQst
			{
				and = "2675;2676;";
				flink = 1;
				flinknextqid = "2678;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267708;
			gtype = 1;
			oklnk = 2;
			area = 267701;
			goal = 267704;
			sort = 267705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 267714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 267701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2793101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

