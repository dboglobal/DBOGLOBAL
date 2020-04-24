CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2387;
	title = 238702;

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
				conv = 238709;
				ctype = 1;
				idx = 3173112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 238708;
				gtype = 1;
				area = 238701;
				goal = 238704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 238705;
				stype = 2;
				taid = 1;
				title = 238702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 238707;
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
				npcidx = "3173112;";
			}
			CDboTSCheckClrQst
			{
				and = "2386;";
				flink = 1;
				flinknextqid = "2393;";
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
			cont = 238708;
			gtype = 1;
			oklnk = 2;
			area = 238701;
			goal = 238704;
			sort = 238705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 238702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 238714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 238701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072302;";
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

