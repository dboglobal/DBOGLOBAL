CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1397;
	title = 139702;

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
				conv = 139709;
				ctype = 1;
				idx = 3142112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 139708;
				gtype = 1;
				area = 139701;
				goal = 139704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 139705;
				stype = 1;
				taid = 1;
				title = 139702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 139714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 139701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1654110;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 139707;
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
				npcidx = "3142112;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1398;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 139708;
			gtype = 1;
			oklnk = 2;
			area = 139701;
			goal = 139704;
			sort = 139705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 139702;
		}
	}
}

