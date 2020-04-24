CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2560;
	title = 256002;

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
				conv = 256009;
				ctype = 1;
				idx = 5101201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 256008;
				gtype = 1;
				area = 256001;
				goal = 256004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 256005;
				stype = 2;
				taid = 1;
				title = 256002;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 256008;
			gtype = 1;
			oklnk = 2;
			area = 256001;
			goal = 256004;
			sort = 256005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 256002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 256014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 256001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751409;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 256007;
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
				npcidx = "5101201;";
			}
			CDboTSCheckClrQst
			{
				and = "2559;";
				flink = 1;
				flinknextqid = "2561;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

