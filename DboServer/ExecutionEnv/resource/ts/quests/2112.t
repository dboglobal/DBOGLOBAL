CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2112;
	title = 211202;

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
				conv = 211209;
				ctype = 1;
				idx = 3031304;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 211208;
				gtype = 1;
				area = 211201;
				goal = 211204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 211205;
				stype = 1;
				taid = 1;
				title = 211202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 211207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3031304;";
			}
			CDboTSCheckClrQst
			{
				and = "2111;";
				flink = 1;
				flinknextqid = "2113;";
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
			cont = 211208;
			gtype = 1;
			oklnk = 2;
			area = 211201;
			goal = 211204;
			sort = 211205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 211202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 211214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 211201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031215;";
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

