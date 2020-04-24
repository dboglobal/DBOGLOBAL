CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1346;
	title = 134602;

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
				conv = 134609;
				ctype = 1;
				idx = 4372211;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 134608;
				gtype = 1;
				area = 134601;
				goal = 134604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 134605;
				stype = 1;
				taid = 1;
				title = 134602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 134607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "4372211;";
			}
			CDboTSCheckClrQst
			{
				and = "1345;";
				flink = 1;
				flinknextqid = "1347;";
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
			cont = 134608;
			gtype = 1;
			oklnk = 2;
			area = 134601;
			goal = 134604;
			sort = 134605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 134602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 134614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 134601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3331103;";
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

