CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1406;
	title = 140602;

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
				conv = 140609;
				ctype = 1;
				idx = 3171103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 140608;
				gtype = 1;
				area = 140601;
				goal = 140604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 140605;
				stype = 2;
				taid = 1;
				title = 140602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 140607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "3171103;";
			}
			CDboTSCheckClrQst
			{
				and = "1394;";
				flink = 1;
				flinknextqid = "1407;";
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
			cont = 140608;
			gtype = 1;
			oklnk = 2;
			area = 140601;
			goal = 140604;
			sort = 140605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 140602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 140614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 140601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8755301;";
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

