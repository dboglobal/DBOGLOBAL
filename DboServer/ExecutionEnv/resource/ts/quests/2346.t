CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2346;
	title = 234602;

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
				conv = 234609;
				ctype = 1;
				idx = 7611105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 234608;
				gtype = 1;
				area = 234601;
				goal = 234604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 234605;
				stype = 1;
				taid = 1;
				title = 234602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 234607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "7611105;";
			}
			CDboTSCheckClrQst
			{
				and = "2344;";
				flink = 1;
				flinknextqid = "2347;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 234608;
			gtype = 1;
			oklnk = 2;
			area = 234601;
			goal = 234604;
			sort = 234605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 234602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 234614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 234601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3811105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

