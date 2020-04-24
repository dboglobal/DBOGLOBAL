CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1706;
	title = 170602;

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
				conv = 170609;
				ctype = 1;
				idx = 5313202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 170608;
				gtype = 1;
				area = 170601;
				goal = 170604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 170605;
				stype = 2;
				taid = 1;
				title = 170602;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 170614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 170601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6311201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 170608;
			gtype = 1;
			oklnk = 2;
			area = 170601;
			goal = 170604;
			sort = 170605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 170602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 170607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "5313202;";
			}
			CDboTSCheckClrQst
			{
				and = "1704;";
				flink = 1;
				flinknextqid = "1708;1723;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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

