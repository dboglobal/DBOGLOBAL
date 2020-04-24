CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 842;
	title = 84202;

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
				conv = 84209;
				ctype = 1;
				idx = 1653107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 84208;
				gtype = 2;
				area = 84201;
				goal = 84204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 84205;
				stype = 2;
				taid = 1;
				title = 84202;
			}
			CDboTSActItem
			{
				iidx0 = 99007;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 84214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 84201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 84207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "1653107;";
			}
			CDboTSCheckClrQst
			{
				and = "841;";
				flink = 1;
				flinknextqid = "843;";
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
			cont = 84208;
			gtype = 2;
			oklnk = 2;
			area = 84201;
			goal = 84204;
			sort = 84205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 84202;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99007;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

