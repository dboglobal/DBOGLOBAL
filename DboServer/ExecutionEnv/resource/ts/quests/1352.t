CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1352;
	title = 135202;

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
				conv = 135209;
				ctype = 1;
				idx = 2801203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 135208;
				gtype = 1;
				area = 135201;
				goal = 135204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 135205;
				stype = 1;
				taid = 1;
				title = 135202;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 135208;
			gtype = 1;
			oklnk = 2;
			area = 135201;
			goal = 135204;
			sort = 135205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 135202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 135214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 135201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1111115;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 135207;
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
				npcidx = "2801203;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1353;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

