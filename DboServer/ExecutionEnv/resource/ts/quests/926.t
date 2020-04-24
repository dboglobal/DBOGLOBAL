CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 926;
	title = 92602;

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
				conv = 92609;
				ctype = 1;
				idx = 6311204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 92608;
				gtype = 1;
				area = 92601;
				goal = 92604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 92605;
				stype = 2;
				taid = 1;
				title = 92602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 92607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 32;
			}
			CDboTSClickNPC
			{
				npcidx = "6311204;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "927;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			cont = 92608;
			gtype = 1;
			oklnk = 2;
			area = 92601;
			goal = 92604;
			sort = 92605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 92602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 92614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 92601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142101;";
			}
		}
	}
}

